from app.db import db


class Personnel(db.Model):
    __tablename__ = 'personnel'
    id = db.Column('id', db.Integer, primary_key=True)
    name = db.Column('name', db.String(60))
    number = db.Column('number', db.Integer)
    speciality = db.Column('speciality', db.String(32))

    def validate(self):
        try:
            int(self.number)
        except:
            raise Exception('Invalid number of personnel.')

    @property
    def serialize(self):
        return {'personnel': {'id': self.id, 'name': self.name, 'number': self.number,
                              'speciality': self.speciality}}

    @property
    def serialize_many2many(self):
        return [item.serialize for item in self.many2many]
