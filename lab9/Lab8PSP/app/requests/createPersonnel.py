from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.personnel import Personnel

logger = app_logger.get_logger(__name__)


@app.route('/createPersonnel', methods=["POST"])
def createPersonnel():
    response = None
    try:
        #logger.info('Handling request: Adding a record to a table')
        Pname = request.args.get('name')
        Pnumber = request.args.get('number')
        Pspeciality = request.args.get('speciality')
        personnel = Personnel(name=Pname, number=Pnumber,
                              speciality=Pspeciality)
        personnel.validate()
        db.session.add(personnel)
        db.session.commit()
        response = jsonify({'success': 'ADD'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response
