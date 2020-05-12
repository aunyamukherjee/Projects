import random

def wheelfortune(consonantlist,vowellist):
    winnings = 0
    consonantguess=""
    vowelguess=""
    #we initialize the values for winnings constonat guess and vowel guess as we will need them later
    phrasebank= open('phrasebank.txt').read().splitlines()
    randomint =random.randint(0,99)
    phrase= phrasebank[randomint].upper()
    # this reads and chooses a random phrase from the phrase bank
    if randomint<=19:
        category= "Before and After"
    elif randomint>19 and randomint<=39:
        category= "Song Lyrics"
    elif randomint>39 and randomint<=59:
        category= "Around the House"
    elif randomint>59 and randomint<=79:
        category="Food and Drink"
    elif randomint>79 and randomint<=99:
        category="Same Name"
    # this assigns what category is associated with which values
    game= "in progress"
    while game == "in progress":
        print("Vowels guessed:",vowellist)
        print("Consonants guessed:",consonantlist)
        print("Your current winnings are:",winnings)
        print("The phrase is:")
        phrasePrint(phrase,consonantlist,vowellist)
        print("The category is:")
        print(category)
        userinput= str(input("Would you like to Spin the Wheel (type ‘spin’), Buy A Vowel (type ‘vowel’), or Solve the Puzzle (type ‘solve’)?:")).upper()
        if userinput=='SPIN':
            winnings,phrase,consonantguess=spinTheWheel(winnings,phrase)
            consonantlist += consonantguess
            #here we add the letter we recieved from the spin function to our ongoing list of consonants
        elif userinput=='VOWEL':
            winnings,phrase,vowelguess=buyAVowel(winnings,phrase)
            vowellist += vowelguess
            #here we add the vowel that was recieved by the vowel function to our ongoing vowel list
        elif userinput=='SOLVE':
            winnings,phrase,game=solveThePuzzle(winnings,phrase,game)
        else:
            print("Sorry I didn't recognise that text. Please try again.")
        # this counter below keeps track of when the user has guessed all the letters
        # assuming that the user does not input letters more than once
        counter=0
        for letter in consonantlist:
            counter += 1
        for letter in vowellist:
            counter += 1
        if counter ==26:
            winnings=0
            print("You've guessed all the letters! Your current balance is",winnings,"and you have lost the game! Try again next time :(")
            game="lost"
            #once the user has guessed all the letters, the game ends


def phrasePrint(phrase,consonantlist,vowellist):
    blanks= []
    # this function displays the correctly guessed letters,
    # and puts dashes for the unguessed letters, as well as spaces for the spaces in the words
    for x in phrase:
        if x in consonantlist:
            blanks += [x]
        elif x in vowellist:
            blanks += [x]
        elif x in string.ascii_uppercase:
            blanks += ['_']
        else:
            blanks += [' ']
    for element in blanks:
        print(element, end=' ')
        #this prints out the elements side by side with space in between

def spinTheWheel(winnings,phrase):
    values= [50,100,100,100,100,100,100,200,200,200,200,250,250,250,500,500,750,750,1000,2000,5000,10000,"bankrupt","bankrupt"]
    spinvalue= values[random.randint(0,23)]
    print("You spun",spinvalue,"!!")
    if spinvalue=="bankrupt":
        winnings=0
        print("Your winnings will be reset to $0")
        consonantguess=""
    else:
        consonantguess= input("Guess a consonant:").upper()
        if consonantguess in "AEIOU":
            print("That's not a consonant! Try again")
            consonantguess= input("Guess a consonant:").upper()
        elif len(consonantguess) != 1:
            print("Please only guess one consonant. Try again:")
            consonantguess= input("Guess a consonant:").upper()
        consonantnumber= phrase.count(consonantguess)
        if consonantnumber==0:
            winnings += -spinvalue
            print("Sorry,",consonantguess,"does not appear in the puzzle. You will have $",spinvalue,"deducted from your winnings.")
        else:
            print("Congrats,",consonantguess,"appears in the phrase",consonantnumber,"time(s)! You’ve won $",spinvalue*consonantnumber)
            winnings += (spinvalue*consonantnumber)
    return(winnings,phrase,consonantguess)

def buyAVowel(winnings,phrase):
    if winnings<250:
        print("You do not have enough to buy a vowel! Sorry!")
    elif winnings>=250:
        vowelguess= input("Ok! $250 will be deducted from your winnings. Which vowel would you like to buy (A, E, I, O, U)?:").upper()
        winnings += -250
        if vowelguess not in "AEIOU":
            print("That's not a vowel! Try again")
            vowelguess= input("Guess a vowel,(A E I O U):").upper()
        elif len(vowelguess) != 1:
            print("Please only guess one vowel. Try again:")
            vowelguess= input("Guess a vowel,(A E I O U):").upper()
        vowelnumber= phrase.count(vowelguess)
        if vowelnumber==0:
            print("Sorry,",vowelguess,"does not appear in the puzzle.")
        else:
            print("Congrats,",vowelguess,"appears in the phrase",vowelnumber,"time(s)!")
    return(winnings,phrase,vowelguess)

def solveThePuzzle(winnings,phrase,game):
    puzzleguess= input("What’s your best guess (be sure to enter your guess with single spaces!)?").upper()
    if puzzleguess==phrase:
        print("That’s correct - you solved the puzzle!")
        print("Congratulations, you’ve won the game! Your winnings are $",winnings)
        print("Thank you for playing the Wheel of Fortune!")
        game="won"
    else:
        print("Sorry, that guess is incorrect! Your winnings will start over at $0")
        winnings=0
    return(winnings,phrase,game)

def main():
    consonantlist=[ ]
    vowellist=[ ]
    print("Welcome to the Wheel of Fortune!")
    wheelfortune(consonantlist,vowellist)

if __name__ == '__main__':
    main()
