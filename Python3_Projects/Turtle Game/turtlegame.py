#Completed through Task 10
# Aunya Mukherjee mukhe074
# I understand this is a graded, individual examination that may not be
# discussed with anyone. I also understand that obtaining solutions or
# partial solutions from outside sources, or discussing
# any aspect of the examination with anyone will result in failing the course.
# I further certify that this program represents my own work and that none of
# it was obtained from any source other than material presented as part of the
# course.

from turtle import *
import tkinter.messagebox
import tkinter
import random
import math
import datetime

screenMinX = -500
screenMinY = -500
screenMaxX = 500
screenMaxY = 500

class LaserBeam(RawTurtle):
    def __init__(self,canvas,x,y,direction,dx,dy):
        super().__init__(canvas)
        self.penup()
        self.goto(x,y)
        self.setheading(direction)
        self.color("Green")
        self.__lifespan = 200
        self.__dx = math.cos(math.radians(direction)) * 2 + dx
        #this is Task 4A
        self.__dy = math.sin(math.radians(direction)) * 2 + dy
        #this is Task 4A
        self.shape("laser")
        #this shape has already been registered.
    def getLifespan(self):
        return self.__lifespan
    def getDx(self):
        return self.__dx
    def getDy(self):
        return self.__dy
    def getRadius(self):
        return 4

    def move(self):
        screen = self.getscreen()
        x = self.xcor()
        y = self.ycor()
        x = (self.__dx + x - screenMinX) % (screenMaxX - screenMinX) + screenMinX
        y = (self.__dy + y - screenMinY) % (screenMaxY - screenMinY) + screenMinY
        self.__lifespan -= 1
        self.goto(x,y)



class Ghost(RawTurtle):
    def __init__(self,canvasobj,dx,dy,x,y,size):
        RawTurtle.__init__(self,canvasobj)
        self.penup()
        self.goto(x,y)
        self.__dx = dx
        self.__dy = dy
        self.__size = size
        if self.__size==3:
            self.shape("blueghost.gif")
        elif self.__size==2:
            self.shape("pinkghost.gif")
    def setDx(self,newdx):
        self.__dx=newdx
    def getDx(self):
        return self.__dx
    def setDy(self,newdy):
        self.__dy= newdy
    def getDy(self):
        return self.__dy
    def getSize(self):
        return self.__size
    #Moves the ghost from its current position to a new position
    def move(self):
        screen = self.getscreen()
        x = self.xcor()
        y = self.ycor()
        x = (self.__dx + x - screenMinX) % (screenMaxX - screenMinX) + screenMinX
        y = (self.__dy + y - screenMinY) % (screenMaxY - screenMinY) + screenMinY


        self.goto(x,y)

    #returns the apprximate "radius" of the Ghost object
    def getRadius(self):
        return self.__size * 10 - 5

class FlyingTurtle(RawTurtle):
    def __init__(self,canvasobj,dx,dy,x,y, size):
        RawTurtle.__init__(self,canvasobj)
        self.penup()
        self.color("purple")
        self.goto(x,y)
        self.__dx = dx
        self.__dy = dy
        self.__size = size
        self.shape("turtle")
    def setDx(self,newdx):
        self.__dx=newdx
    def getDx(self):
        return self.__dx
    def setDy(self,newdy):
        self.__dy= newdy
    def getDy(self):
        return self.__dy
    def move(self):
        screen = self.getscreen()
        x = self.xcor()
        y = self.ycor()

        x = (self.__dx + x - screenMinX) % (screenMaxX - screenMinX) + screenMinX
        y = (self.__dy + y - screenMinY) % (screenMaxY - screenMinY) + screenMinY

        self.goto(x,y)

    def turboBoost(self):
        angle = self.heading()
        x = math.cos(math.radians(angle))
        y = math.sin(math.radians(angle))
        if self.__dx ==0 and self.__dy==0:
            self.__dx = self.__dx + x
            self.__dy = self.__dy + y
        else:
            self.__dx = 1.5*self.__dx
            self.__dy = 1.5*self.__dy

    def stopTurtle(self):
        angle = self.heading()
        self.__dx = 0
        self.__dy = 0

    def getRadius(self):
        return 2

def intersect(obj1,obj2):
    result= False
    x1 = obj1.xcor()
    y1 = obj1.ycor()
    x2 = obj2.xcor()
    y2 = obj2.ycor()
    distance = math.sqrt(((x2-x1)**2) + ((y2-y1)**2))
    if distance <= (obj1.getRadius() + obj2.getRadius()):
        result= True
    return(result)



def main():
    # Start by creating a RawTurtle object for the window.
    firstwindow = tkinter.Tk()
    firstwindow.title("Turtle Saves the World!")
    canvas = ScrolledCanvas(firstwindow,600,600,600,600)
    canvas.pack(side = tkinter.LEFT)
    t = RawTurtle(canvas)

    screen = t.getscreen()
    screen.setworldcoordinates(screenMinX,screenMinY,screenMaxX,screenMaxY)
    screen.register_shape("blueghost.gif")
    screen.register_shape("pinkghost.gif")
    screen.register_shape("laser",((-2,-4),(-2,4),(2,4),(2,-4)))
    frame = tkinter.Frame(firstwindow)
    frame.pack(side = tkinter.RIGHT,fill=tkinter.BOTH)
    scoreVal = tkinter.StringVar()
    scoreVal.set("0")
    scoreTitle = tkinter.Label(frame,text="Score")
    scoreTitle.pack()
    scoreFrame = tkinter.Frame(frame,height=2, bd=1, relief=tkinter.SUNKEN)
    scoreFrame.pack()
    score = tkinter.Label(scoreFrame,height=2,width=20,textvariable=scoreVal,fg="Yellow",bg="black")
    score.pack()
    livesTitle = tkinter.Label(frame, text="Extra Lives Remaining")
    livesTitle.pack()
    livesFrame = tkinter.Frame(frame,height=30,width=60,relief=tkinter.SUNKEN)
    livesFrame.pack()
    livesCanvas = ScrolledCanvas(livesFrame,150,40,150,40)
    livesCanvas.pack()
    livesTurtle = RawTurtle(livesCanvas)
    livesTurtle.ht()
    livesScreen = livesTurtle.getscreen()
    life1 = FlyingTurtle(livesCanvas,0,0,-35,0,3)
    life2 = FlyingTurtle(livesCanvas,0,0,0,0,3)
    life3 = FlyingTurtle(livesCanvas,0,0,35,0,3)
    lives = [life1, life2, life3]
    t.ht()

    screen.tracer(10)

    #Tiny Turtle!
    flyingturtle = FlyingTurtle(canvas,0,0,(screenMaxX-screenMinX)/2+screenMinX,(screenMaxY-screenMinY)/2 + screenMinY,3)
    #A list to keep track of all the ghosts
    ghosts = []
    lasers=[]
    deadlasers= []
    hitghosts=[]

    #Create some ghosts and randomly place them around the screen
    for numofghosts in range(6):
        dx = random.random()*6  - 4
        dy = random.random()*6  - 4
        x = random.random() * (screenMaxX - screenMinX) + screenMinX
        y = random.random() * (screenMaxY - screenMinY) + screenMinY

        ghost = Ghost(canvas,dx,dy,x,y,3)

        ghosts.append(ghost)

    def fireLaser():
        speed= math.sqrt((flyingturtle.getDy())**2+(flyingturtle.getDx())**2)
        #for task 9
        angle= flyingturtle.heading()
        LaserBeamobj= LaserBeam(canvas,flyingturtle.xcor(),flyingturtle.ycor(),flyingturtle.heading(),speed*math.cos(math.radians(angle)),speed*math.sin(math.radians(angle)))
        lasers.append(LaserBeamobj)


    def play():
        #start counting time for the play function
        ##LEAVE THIS AT BEGINNING OF play()
        start = datetime.datetime.now()
        if len(ghosts)== 0:
                tkinter.messagebox.showinfo("You Win!!", "You saved the world!")
                return
        # Move the turtle
        flyingturtle.move()

        for each_laser in lasers:
            if each_laser.getLifespan() !=0:
                each_laser.move()
            if each_laser.getLifespan() == 0:
                deadlasers.append(each_laser)
                lasers.remove(each_laser)
                each_laser.goto(-screenMinX*2, -screenMinY*2)
                each_laser.ht()

        #Move the ghosts
        for each_ghost in ghosts:
            each_ghost.move()


        for each_ghost in ghosts:
            for each_laser in lasers:
                if each_ghost.getSize()==3:
                    if intersect(each_laser,each_ghost) == True:
                        deadlasers.append(each_laser)
                        lasers.remove(each_laser)
                        speed= math.sqrt((each_ghost.getDy())**2+(each_ghost.getDx())**2)
                        heading= each_laser.heading()
                        ghosts.remove(each_ghost)
                        hitghosts.append(each_ghost)
                        each_laser.goto(-screenMinX*2, -screenMinY*2)
                        each_laser.ht()
                        each_ghost.ht()
                        #for task 10
                        pinkghost1= Ghost(canvas,(math.cos(math.radians(heading-25.0))*speed),(math.sin(math.radians(heading-25.0))*speed),each_ghost.xcor(),each_ghost.ycor(),2)
                        pinkghost2= Ghost(canvas,(math.cos(math.radians(heading+25.0))*speed),(math.sin(math.radians(heading+25.0))*speed),each_ghost.xcor(),each_ghost.ycor(),2)
                        ghosts.append(pinkghost1)
                        ghosts.append(pinkghost2)
                        each_ghost.goto(-screenMinX*2, -screenMinY*2)
                        temp=20+int(scoreVal.get())
                        scoreVal.set(str(temp))
                elif each_ghost.getSize()==2:
                    if intersect(each_laser,each_ghost) == True:
                        deadlasers.append(each_laser)
                        lasers.remove(each_laser)
                        hitghosts.append(each_ghost)
                        each_laser.goto(-screenMinX*2, -screenMinY*2)
                        each_laser.ht()
                        each_ghost.ht()
                        ghosts.remove(each_ghost)
                        each_ghost.goto(-screenMinX*2, -screenMinY*2)
                        temp=30+int(scoreVal.get())
                        scoreVal.set(str(temp))

        for each_ghost in ghosts:
            if each_ghost.getSize()==2:
                if intersect(each_ghost,flyingturtle) == True:
                    ghosts.remove(each_ghost)
                    hitghosts.append(each_ghost)
                    each_ghost.goto(-screenMinX*2, -screenMinY*2)
                    each_ghost.ht()
                    if len(lives)==0:
                        tkinter.messagebox.showinfo("You Loose!!", "The world died...")
                        flyingturtle.ht()
                        return
                    turtlego=lives[0]
                    turtlego.goto(-screenMinX*2, -screenMinY*2)
                    turtlego.ht()
                    lives.remove(lives[0])
                    tkinter.messagebox.showwarning( "Uh-Oh","You Lost a Life!")
                    flyingturtle.stopTurtle()
            elif each_ghost.getSize()==3:
                if intersect(each_ghost,flyingturtle) == True:
                    ghosts.remove(each_ghost)
                    hitghosts.append(each_ghost)
                    each_ghost.goto(-screenMinX*2, -screenMinY*2)
                    each_ghost.ht()
                    if len(lives)==0:
                        tkinter.messagebox.showinfo("You Loose!!", "The world died...")
                        flyingturtle.ht()
                        return
                    turtlego=lives[0]
                    turtlego.goto(-screenMinX*2, -screenMinY*2)
                    turtlego.ht()
                    lives.remove(lives[0])
                    tkinter.messagebox.showwarning( "Uh-Oh","You Lost a Life!")
                    flyingturtle.stopTurtle()

        #stop counting time for the play function
        ##LEAVE THIS AT END OF ALL CODE IN play()
        end = datetime.datetime.now()
        duration = end - start

        millis = duration.microseconds / 1000.0

        # Set the timer to go off again
        screen.ontimer(play,int(10-millis))
    # Set the timer to go off the first time in 5 milliseconds
    screen.ontimer(play, 5)

    #Turn turtle 7 degrees to the left
    def turnLeft():
        flyingturtle.setheading(flyingturtle.heading()+7)
    def turnRight():
        flyingturtle.setheading(flyingturtle.heading()-7)

    #turboBoost turtle
    def forward():
        flyingturtle.turboBoost()

    #stop Turtle
    def stop():
        flyingturtle.stopTurtle()

    #Call functions above when pressing relevant keys
    screen.onkeypress(turnLeft,"Left")
    screen.onkeypress(turnRight,"Right")
    screen.onkeypress(forward,"Up")
    screen.onkeypress(stop,"Down")
    screen.onkeypress(fireLaser,"")

    screen.listen()
    tkinter.mainloop()

if __name__ == "__main__":
    main()
