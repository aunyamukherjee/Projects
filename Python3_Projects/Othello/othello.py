# Aunya Mukherjee mukhe074
# I understand this is a graded, individual examination that may not be
# discussed with anyone. I also understand that obtaining solutions or
# partial solutions from outside sources, or discussing
# any aspect of the examination with anyone will result in failing the course.
# I further certify that this program represents my own work and that none of
# it was obtained from any source other than material presented as part of the
# course.
import turtle
import random
def boardConversion():
    row={0:195,1:140,2:85,3:30,4:-25,5:-80,6:-135,7:-190}
    col={0:-150,1:-105,2:-60,3:-15,4:30,5:75,6:120,7:165}
    return(row,col)
def othello():
    game="inprogress"
    board=[['0','0','0','0','0','0','0','0'],['0','0','0','0','0','0','0','0'],['0','0','0','0','0','0','0','0'],['0','0','0',"w","b",'0','0','0'],['0','0','0',"b","w",'0','0','0'],['0','0','0','0','0','0','0','0'],['0','0','0','0','0','0','0','0'],['0','0','0','0','0','0','0','0']]
    row,col=boardConversion()
    turtle.hideturtle()
    turtle.setworldcoordinates(-235,-235,235,235)
    turtle.speed(0)
    turtle.color("green")
    turtle.shape("square")
    turtle.turtlesize(3,3)
    turtle.penup()
    turtle.setx(-150)
    turtle.sety(-190)
    for x in range(0,4):
        for x in range(0,7):
            turtle.stamp()
            turtle.forward(45)
        turtle.stamp()
        turtle.left(90)
        turtle.forward(55)
        turtle.left(90)
        for x in range(0,7):
            turtle.stamp()
            turtle.forward(45)
        turtle.stamp()
        turtle.right(90)
        turtle.forward(55)
        turtle.right(90)
    turtle.hideturtle()
    turtle.right(90)
    turtle.forward(25)
    turtle.left(90)
    turtle.color("black")
    for x in range(0,8):
        turtle.write(x)
        turtle.forward(45)
    turtle.right(90)
    turtle.forward(35)
    turtle.right(90)
    turtle.forward(395)
    turtle.left(90)
    for x in range(0,8):
        turtle.write(x)
        turtle.forward(55)
    whitepiece= turtle.Turtle()
    blackpiece= turtle.Turtle()
    whitepiece.speed(2)
    whitepiece.penup()
    whitepiece.goto(-15,30)
    whitepiece.shape("circle")
    whitepiece.color("white")
    whitepiece.turtlesize(2,2)
    whitepiece.stamp()
    whitepiece.goto(30,-25)
    whitepiece.stamp()
    blackpiece.speed(2)
    blackpiece.penup()
    blackpiece.shape("circle")
    blackpiece.color("black")
    blackpiece.penup()
    blackpiece.turtlesize(2,2)
    blackpiece.goto(30,30)
    blackpiece.stamp()
    blackpiece.goto(-15,-25)
    blackpiece.stamp()
    while game=="inprogress":
        if getValidMoves(board,"black")!=[ ]:
            board,blackpiece,row,col,game=userturn(board,blackpiece,row,col,game)
        if getValidMoves(board,"white")!=[ ] and game=="inprogress":
            board,whitepiece,row,col=selectNextPlay(board,whitepiece,row,col)
        if getValidMoves(board,"black")==[ ] and getValidMoves(board,"white")==[ ]:
            game="over"
            gameOver(game,board)


def gameOver(game,board):
    winner={"b":0,"w":0}
    for x in range(8):
        for y in range(8):
            if board[x][y]=="b":
                winner["b"] += 1
            if board[x][y]=="w":
                winner["w"] += 1
    turtle.color("purple")
    if winner["b"]>winner["w"]:
        turtle.penup()
        turtle.goto(0,100)
        turtle.write("Black Wins!!!",align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,0)
        turtle.write("Black:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,0)
        turtle.write( winner["b"],align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,-100)
        turtle.write("White:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,-100)
        turtle.write(winner["w"],align="center",font=("Arial", 80, "bold"))
    elif winner["b"]<winner["w"]:
        turtle.penup()
        turtle.goto(0,100)
        turtle.write("White Wins!!!",align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,0)
        turtle.write("Black:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,0)
        turtle.write( winner["b"],align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,-100)
        turtle.write("White:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,-100)
        turtle.write(winner["w"],align="center",font=("Arial", 80, "bold"))
    else:
        turtle.penup()
        turtle.goto(0,100)
        turtle.write("Its a Tie!!!",align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,0)
        turtle.write("Black:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,0)
        turtle.write( winner["b"],align="center",font=("Arial", 80, "bold"))
        turtle.goto(-100,-100)
        turtle.write("White:",align="center",font=("Arial", 80, "bold"))
        turtle.goto(100,-100)
        turtle.write(winner["w"],align="center",font=("Arial", 80, "bold"))
    return(game,board)

def userturn(board,blackpiece,row,col,game):
    color="black"
    textinput=str(turtle.textinput("Your move! Your token is black.","Enter a valid row,column:"))
    if textinput=="":
        game="over"
        gameOver(game,board)
    else:
        rowvalue=int(textinput[0])
        columnvalue=int(textinput[2])
        board,rowvalue,columnvalue,neighbours=neighbourhood(board,rowvalue,columnvalue)
        result,stampline=isValidMove(board,rowvalue,columnvalue,neighbours,color)
        if result==False:
            userturn(board,blackpiece,row,col,game)
        else:
            board[rowvalue][columnvalue]="b"
            blackpiece.goto(col[columnvalue],row[rowvalue])
            blackpiece.stamp()
            for point in stampline:
                board[point[0]][point[1]]="b"
                blackpiece.goto(col[point[1]],row[point[0]])
                blackpiece.stamp()
    return(board,blackpiece,row,col,game)

def selectNextPlay(board,whitepiece,row,col):
    color="white"
    validMoves=getValidMoves(board,color)
    computermove=validMoves[random.randint(0,len(validMoves)-1)]
    board,rowvalue,columnvalue,neighbours=neighbourhood(board,computermove[0],computermove[1])
    result,stampline=isValidMove(board,rowvalue,columnvalue,neighbours,color)
    board[computermove[0]][computermove[1]]="w"
    whitepiece.goto(col[columnvalue],row[rowvalue])
    whitepiece.stamp()
    for point in stampline:
        board[point[0]][point[1]]="w"
        whitepiece.goto(col[point[1]],row[point[0]])
        whitepiece.stamp()
    return(board,whitepiece,row,col)

def getValidMoves(board,color):
    validMoves=[ ]
    for x in range(0,8):
        for y in range(0,8):
            rowvalue=x
            columnvalue=y
            board,rowvalue,columnvalue,neighbours=neighbourhood(board,rowvalue,columnvalue)
            result,stampline=isValidMove(board,rowvalue,columnvalue,neighbours,color)
            if result==True:
                validMoves+=[(x,y)]
    return(validMoves)

def neighbourhood(board,rowvalue,columnvalue):
    if rowvalue not in [1,2,3,4,5,6,7,0] or columnvalue not in [1,2,3,4,5,6,7,0]:
        neighbours=[0]
    elif board[rowvalue][columnvalue]=="0":
        if rowvalue==0 and columnvalue==0:
            neighbours=[(rowvalue+1,columnvalue),(rowvalue,columnvalue+1),(rowvalue+1,columnvalue+1)]
        elif rowvalue==7 and columnvalue==0:
            neighbours=[(rowvalue-1,columnvalue),(rowvalue,columnvalue+1),(rowvalue-1,columnvalue+1)]
        elif rowvalue==7 and columnvalue==7:
            neighbours=[(rowvalue-1,columnvalue),(rowvalue,columnvalue-1),(rowvalue-1,columnvalue-1)]
        elif rowvalue==0 and columnvalue==7:
            neighbours=[(rowvalue+1,columnvalue),(rowvalue,columnvalue-1),(rowvalue+1,columnvalue-1)]
        elif rowvalue==0:
            neighbours=[(rowvalue,columnvalue-1),(rowvalue+1,columnvalue-1),(rowvalue+1,columnvalue),(rowvalue+1,columnvalue+1),(rowvalue,columnvalue+1)]
        elif rowvalue==7:
            neighbours=[(rowvalue,columnvalue-1),(rowvalue-1,columnvalue-1),(rowvalue-1,columnvalue),(rowvalue-1,columnvalue+1),(rowvalue,columnvalue+1)]
        elif columnvalue==0:
            neighbours=[(rowvalue-1,columnvalue),(rowvalue-1,columnvalue+1),(rowvalue,columnvalue+1),(rowvalue+1,columnvalue+1),(rowvalue+1,columnvalue)]
        elif columnvalue==7:
            neighbours=[(rowvalue-1,columnvalue),(rowvalue-1,columnvalue-1),(rowvalue,columnvalue-1),(rowvalue+1,columnvalue-1),(rowvalue+1,columnvalue)]
        else:
            neighbours=[(rowvalue,columnvalue-1),(rowvalue,columnvalue+1),(rowvalue-1,columnvalue-1),(rowvalue-1,columnvalue),(rowvalue-1,columnvalue+1),(rowvalue+1,columnvalue-1),(rowvalue+1,columnvalue),(rowvalue+1,columnvalue+1)]
    else:
        neighbours=[0]
    return(board,rowvalue,columnvalue,neighbours)

def isValidMove(board,rowvalue,columnvalue,neighbours,color):
    stampline= [ ]
    result=False
    if neighbours!= [0]:
        for coord in neighbours:
            candidateline=[ ]
            if color=="black":
                if board[coord[0]][coord[1]]=="w":
                    newrow=coord[0]
                    newcolumn=coord[1]
                    rowdelta=coord[0]-rowvalue
                    columndelta= coord[1]-columnvalue
                    while newrow != -1 and newrow != 8 and newcolumn != -1 and newcolumn != 8 and board[newrow][newcolumn]=="w":
                        candidateline += [(newrow,newcolumn)]
                        newrow+= rowdelta
                        newcolumn+=columndelta
                    if newrow != -1 and newrow != 8 and newcolumn != -1 and newcolumn != 8 and board[newrow][newcolumn]=="b":
                        result=True
                        for elem in candidateline:
                            stampline+=[elem]
            elif color=="white":
                if board[coord[0]][coord[1]]=="b":
                    newrow=coord[0]
                    newcolumn=coord[1]
                    rowdelta=coord[0]-rowvalue
                    columndelta= coord[1]-columnvalue
                    while newrow != -1 and newrow != 8 and newcolumn != -1 and newcolumn != 8 and board[newrow][newcolumn]=="b":
                        candidateline += [(newrow,newcolumn)]
                        newrow+= rowdelta
                        newcolumn+=columndelta
                    if newrow != -1 and newrow != 8 and newcolumn != -1 and newcolumn != 8 and board[newrow][newcolumn]=="w":
                        result=True
                        for elem in candidateline:
                            stampline+= [elem]
    return(result,stampline)

def main():
    othello()

if __name__ == '__main__':
    main()
