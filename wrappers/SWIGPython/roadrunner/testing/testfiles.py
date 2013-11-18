import pkgutil
import roadrunner

def get_data(resource):
    """OB
    get the contents of a testing resource file.
    """
    return pkgutil.get_data(__name__, resource)


def get_roadrunner(resource):
    """
    return a RoadRunner instance loaded with one of the test files.
    """

    data = get_data(resource)
    r = roadrunner.RoadRunner()
    r.load(data)
    return r

