import pkgutil
import roadrunner

def getData(resource):
    """
    get the contents of a testing resource file.
    """
    return pkgutil.get_data(__name__, resource)


def getRoadRunner(resource):
    """
    return a RoadRunner instance loaded with one of the test files.
    """

    data = pkgutil.get_data(__name__, resource)
    r = roadrunner.RoadRunner()
    r.load(data)
    return r

