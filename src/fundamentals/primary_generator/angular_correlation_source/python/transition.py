ELECTRIC = -1
EM_UNKNOWN = 0
MAGNETIC = 1

class Transition:
    def __init__(self, em, t_L, emp, t_Lp, de):
        self.em_char = em
        self.two_L = t_L
        self.em_charp = emp
        self.two_Lp = t_Lp
        self.delta = de