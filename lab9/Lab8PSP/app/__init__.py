from flask import Flask
import pymysql
pymysql.install_as_MySQLdb()

app = Flask(__name__)

from app.views import main

from app.requests import getPersonnel
from app.requests import deletePersonnel
from app.requests import createPersonnel
from app.requests import updatePersonnel
