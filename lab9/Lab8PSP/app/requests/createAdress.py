from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.adress import Adress

logger = app_logger.get_logger(__name__)


@app.route('/createAdress', methods=["POST"])
def createPersonnel():
    response = None
    try:
        logger.info('Handling request: Adding a record to a table')
        Pname = request.args.get('name')
        Pnumber = request.args.get('number')
        Pcharacteristic = request.args.get('characteristic')
        adress = Adress(name=Pname, number=Pnumber,
                              characteristic=Pcharacteristic)
        adress.validate()
        db.session.add(adress)
        db.session.commit()
        response = jsonify({'success': 'Record added successfully.'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response
