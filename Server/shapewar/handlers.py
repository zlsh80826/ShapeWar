import logging
import tornado.web
from sqlalchemy.exc import IntegrityError
from sqlalchemy.orm.exc import NoResultFound
from .database import DBHandlerMixin, User


logger = logging.getLogger(__name__)


class RequestHandler(DBHandlerMixin, tornado.web.RequestHandler):

    # user related stuff

    def get_current_user(self, user_cookie=None):
        if user_cookie is None:
            user_cookie = self.get_secure_cookie('user')
        if user_cookie is None:
            return None
        else:
            with self.get_session() as session:
                try:
                    return session.query(User)\
                        .filter(User.username == user_cookie).one()
                except NoResultFound:
                    return None

    def set_current_user(self, user):
        self.set_secure_cookie('user', user)

    def logout_user(self):
        self.clear_cookie('user')


class APIHandler(RequestHandler):

    def write_error(self, status_code, exc_info=None):
        self.finish({
            'error': {
                'message': 'HTTP {}: {}'.format(status_code, self._reason)
            }
        })


class APILogin(APIHandler):

    def post(self):
        username = self.get_body_argument('username')
        password = self.get_body_argument('password')
        with self.get_session() as session:
            try:
                user = session.query(User) \
                    .filter(User.username == username).one()
            except NoResultFound:
                self.finish(
                    {'error': {'message': 'no such user'}}
                )
            else:
                if user.verify_password(password):
                    self.finish({'token': "NotImplemented"})
                else:
                    self.finish({
                        'error': {'message': 'invalid passwored'}
                    })


class APIRegister(APIHandler):

    def post(self):
        username = self.get_body_argument('username')
        password = self.get_body_argument('password')
        user = User(username=username, password=password)
        try:
            with self.get_session() as session:
                session.add(user)
        except IntegrityError:
            self.finish({'error': {'message': 'username is used already'}})
        else:
            self.set_current_user(username)
            self.finish({'token': "NotImplemented"})


all_handlers = [
    ('/api/login', APILogin),
    ('/api/register', APIRegister),
]
