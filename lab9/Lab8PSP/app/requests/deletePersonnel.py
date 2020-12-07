from flask import request, jsonify
from app import app, app_logger
from app.db import db
from app.models.personnel import Personnel

logger = app_logger.get_logger(__name__)


@app.route('/deletePersonnel', methods=["POST"])
def deletePersonnel():
    response = None
    try:
        #logger.info('Handling request: Delete record by id')
        Pid = request.args.get('id')
        db.session.query(Personnel).filter_by(id=Pid).delete()
        db.session.commit()
        response = jsonify({'success': 'Deleted!'})
        response.status_code = 200
    except Exception as e:
        db.session.rollback()
        response = jsonify({'error': 'The request had invalid values.'})
        response.status_code = 400
        logger.error(str(e))
    finally:
        return response