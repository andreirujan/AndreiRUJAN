from flask import Flask, render_template, redirect, url_for, request
from flask_bootstrap import Bootstrap5
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column
from sqlalchemy import Boolean, Integer, String, DateTime
from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField, DateField
from wtforms.validators import DataRequired
from flask_wtf import FlaskForm
from wtforms import DateField, SubmitField
from wtforms.validators import DataRequired


app = Flask(__name__)
app.config['SECRET_KEY'] = '8BYkEfBA6O6donzWlSihBXox7C0sKR6b'
Bootstrap5(app)

# CREATE DB
class Base(DeclarativeBase):
    pass

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///school_catalog.db'
db = SQLAlchemy(model_class=Base)
db.init_app(app)

# CREATE STUDENT TABLE
class Student(db.Model):
    id = mapped_column(Integer, primary_key=True)
    name = mapped_column(String(250), nullable=False)
    age = mapped_column(Integer, nullable=False)

class Attendance(db.Model):
    id = mapped_column(Integer, primary_key=True)
    student_id = mapped_column(Integer, db.ForeignKey('student.id'))
    student = db.relationship('Student', back_populates="attendances")
    date = mapped_column(DateTime, nullable=False)
    present = mapped_column(Boolean, nullable=False)

Student.attendances = db.relationship('Attendance', back_populates="student")

with app.app_context():
    db.create_all()

# FORMS
class CreateStudentForm(FlaskForm):
    name = StringField("Student Name", validators=[DataRequired()])
    age = StringField("Student Age", validators=[DataRequired()])
    submit = SubmitField("Add Student")

class AddAttendanceForm(FlaskForm):
    date = DateField("Date", format='%Y-%m-%d', validators=[DataRequired()])
    present = SubmitField("Mark Present")
    absent = SubmitField("Mark Absent")
    excused = SubmitField("Mark Excused")  # Buton pentru invocare


@app.route("/", methods=["GET", "POST"])
def home():
    students = Student.query.all()
    return render_template("index.html", students=students)

@app.route("/add_student", methods=["GET", "POST"])
def add_student():
    form = CreateStudentForm()
    if form.validate_on_submit():
        new_student = Student(
            name=form.name.data,
            age=int(form.age.data)
        )
        db.session.add(new_student)
        db.session.commit()
        return redirect(url_for("home"))
    return render_template("add_student.html", form=form)

@app.route("/delete_student/<int:id>")
def delete_student(id):
    student = Student.query.get_or_404(id)
    db.session.delete(student)
    db.session.commit()
    return redirect(url_for("home"))

@app.route("/student/<int:id>", methods=["GET", "POST"])
def student_detail(id):
    student = Student.query.get_or_404(id)
    form = AddAttendanceForm()

    if form.validate_on_submit():
        # Verificăm ce buton a fost apăsat
        if "present" in request.form:
            present_value = True  # Elevul este prezent
            excused_value = False  # Nu este invocat
        elif "absent" in request.form:
            present_value = False  # Elevul este absent
            excused_value = False  # Nu este invocat
        elif "excused" in request.form:
            present_value = True  # Elevul este marcat ca prezent
            excused_value = True  # Marcat ca invocat

        # Creăm un nou record de prezență
        attendance = Attendance(
            student_id=student.id,
            date=form.date.data,
            present=present_value,
            excused=excused_value
        )
        db.session.add(attendance)
        db.session.commit()
        return redirect(url_for("student_detail", id=student.id))

    attendances = Attendance.query.filter_by(student_id=id).all()
    return render_template("student_detail.html", student=student, form=form, attendances=attendances)



if __name__ == '__main__':
    app.run(debug=True)
