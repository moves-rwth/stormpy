class StormError(Exception):
    """
    Base class for exceptions in Storm.
    """

    def __init__(self, message):
        """
        Constructor.
        :param message: Error message.
        """
        self.message = "Storm: " + message
