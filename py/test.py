from Grid import Grid 

test = Grid([None, None])

def t(G):
    print(G.getState())
    G.setState("SOLVED")
    print(G.getState())

t(test)
