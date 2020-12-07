from flask_sqlalchemy import SQLAlchemy
from app import app, settings, app_logger


logger = app_logger.get_logger(__name__)

data = settings.get_db_config()

dbtype = data['dbtype']
user = data['user']
password = data['password']
host = data['host']
database = data['database']


try:
    app.config['SQLALCHEMY_DATABASE_URI'] = f'{dbtype}://{user}:{password}@{host}/{database}'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db = SQLAlchemy(app)
except Exception as e:
    logger.error(str(e))