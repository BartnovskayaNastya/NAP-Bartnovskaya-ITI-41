from flask import jsonify
from app import app, app_logger
from app.db import db
from app.models.personnel import Personnel

logger = app_logger.get_logger(__name__)

@app.route('/getPersonnel', methods=["GET"])
def getPersonnel():
    try:
        logger.info('Handling request: Getting a list of records')
        list = db.session.query(Personnel)
        response = jsonify([i.serialize for i in list.all()])
        return response
    except Exception as e:
        logger.error(str(e))