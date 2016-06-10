import binascii
import contextlib
import hashlib
import logging
import os
from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import sessionmaker
from sqlalchemy.ext.declarative import as_declarative, declared_attr


logger = logging.getLogger(__name__)


@as_declarative()
class Base:
    """Base class for ORM objects"""

    @declared_attr
    def __tablename__(cls):
        return cls.__name__.lower()

    id = Column(Integer, primary_key=True)


#
# Mixin classes
#


class DBAppMixin:

    def __init__(self, *args, database_engine, **kwargs):
        super().__init__(*args, **kwargs)
        self.database_engine = database_engine
        self.session_class = sessionmaker(self.database_engine)


class DBHandlerMixin:

    @contextlib.contextmanager
    def get_session(self):
        session = self.application.session_class()
        try:
            yield session
            session.commit()
        except:
            session.rollback()
            raise
        finally:
            session.close()


#
# User
#


def hash_password(password, salt):
    """hashes the password with the given salt, return hex bytes"""

    dk = hashlib.pbkdf2_hmac(
        hash_name='sha256',
        password=password.encode('utf-8'),
        salt=salt,
        iterations=100000
    )
    return binascii.b2a_hex(dk)


def make_password_hash(password):
    """makes a password hash for the given password,
    using salt from os.urandom

    returns tuple of (hash, salt), both in hex bytes"""

    salt = os.urandom(8)
    return hash_password(password, salt), binascii.b2a_hex(salt)


def verify_password(password, salt, hash_):
    return hash_ == hash_password(
        password,
        binascii.a2b_hex(salt)
    )


class User(Base):
    """User class"""

    username = Column(String(80), nullable=False, unique=True)

    password_hash = Column(String(64))
    password_salt = Column(String(8))

    def __init__(self, *, username, password):
        self.username = username
        self.set_password(password)

    def __repr__(self):
        return '<User %r>' % self.username

    def set_password(self, password):
        self.password_hash, self.password_salt = make_password_hash(password)

    def verify_password(self, password):
        return verify_password(
            password,
            self.password_salt,
            self.password_hash
        )
