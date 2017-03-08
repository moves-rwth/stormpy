from . import action

class State:
    """ Represents a state in the matrix """

    def __init__(self, id, matrix):
        """ Initialize
        :param id: Id of the state
        :param matrix: Corresponding matrix
        """
        self.id = id - 1
        self.matrix = matrix

    def __iter__(self):
        return self

    def __next__(self):
        if self.id >= self.matrix.nr_row_groups - 1:
            raise StopIteration
        else:
            self.id += 1
            return self

    def __str__(self):
        return "{}".format(self.id)

    def actions(self):
        """ Get actions associated with the state
        :return List of actions
        """
        row_group_indices = self.matrix._row_group_indices
        start = row_group_indices[self.id]
        end = row_group_indices[self.id+1]
        return action.Action(start, end, 0, self.matrix)
