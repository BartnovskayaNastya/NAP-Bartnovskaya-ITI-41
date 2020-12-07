from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.personnel import Personnel

logger = app_logger.get_logger(__name__)


@app.route('/updatePersonnel', methods=["POST"])
def updatePersonnel():
    response = None
    try:
        logger.info('Handling request: UPDARE')
        idP = request.args.get('id')
        nameP = request.args.get('name')
        numberP = request.args.get('number')
        specialityP = request.args.get('speciality')
        personnel = db.session.query(Personnel).get(idP)
        personnel.name = nameP
        personnel.number = numberP
        personnel.speciality = specialityP
        personnel.validate()
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
