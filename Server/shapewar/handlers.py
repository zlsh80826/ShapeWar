import tornado.web
from sqlalchemy.exc import IntegrityError
from sqlalchemy.orm.exc import NoResultFound
from .database import DBHandlerMixin, User


class RequestHandler(DBHandlerMixin, tornado.web.RequestHandler):
    pass


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
                    self.finish('ok XXX')
                else:
                    self.finish({
                        'error': {'message': 'invalid passwored'}
                    })


class APIRegister(APIHandler):

    def post(self):
        user = User(
            username=self.get_body_argument('username'),
            password=self.get_body_argument('password')
        )
        try:
            with self.get_session() as session:
                session.add(user)
        except IntegrityError:
            self.finish({'error': {'message': 'username is used already'}})
        else:
            self.finish('ok XXX')


all_handlers = [
    ('/api/login', APILogin),
    ('/api/register', APIRegister),
]
