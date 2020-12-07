from app.db import db


class Adress(db.Model):
    __tablename__ = 'adress'
    id = db.Column('id', db.Integer, primary_key=True)
    name = db.Column('name', db.String(60))
    number = db.Column('number', db.Integer)
    characteristic = db.Column('characteristic', db.String(32))

    def validate(self):
        try:
            int(self.number)
        except:
            raise Exception('Invalid number of personnel.')

    @property
    def serialize(self):
        return {'adress': {'id': self.id, 'name': self.name, 'number': self.number,
                           'characteristic': self.characteristic}}

    @property
    def serialize_many2many(self):
        return [item.serialize for item in self.many2many]
