POSITIVE = 1
NEGATIVE = -1
PARITY_UNKNOWN = 0

class State:
    def __init__(self, t_J, par):
        self.two_J = t_J
        self.parity = par