from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.adress import Adress

logger = app_logger.get_logger(__name__)


@app.route('/updateAdress', methods=["POST"])
def updateAdress():
    response = None
    try:
        logger.info('Handling request: Updating a record in a table')
        idP = request.args.get('id')
        nameP = request.args.get('name')
        numberP = request.args.get('number')
        characteristicP = request.args.get('characteristic')
        adress = db.session.query(Adress).get(idP)
        adress.name = nameP
        adress.number = numberP
        adress.characteristic = characteristicP
        adress.validate()
        db.session.commit()
        response = jsonify({'success': 'Record successfully updated.'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response
