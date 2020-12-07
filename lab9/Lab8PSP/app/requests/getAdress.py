from flask import jsonify
from app import app, app_logger
from app.db import db
from app.models.adress import Adress

logger = app_logger.get_logger(__name__)

@app.route('/getAdress', methods=["GET"])
def getAdress():
    try:
        logger.info('Handling request: Getting a list of records')
        list = db.session.query(Adress)
        response = jsonify([i.serialize for i in list.all()])
        return response
    except Exception as e:
        logger.error(str(e))