import random
import time
import sys
import re

delays = 0 # seconds

# interactive neural optimizer
# flam

# begin config

neurons = 5 # ok so this one should always be 5
digits = 5  # length of seven-segment display

minwordlen = 4  # min length of a word for scoring
maxwordlen = 50 # max (dont load longer ones)

minweight = -33
maxweight = 33
minthresh = 2
maxthresh = 10

maxoutputsize = 42

maxranddepth = 40

# config end

mask = "ns" # neuron and synapse mutations enabled at runtime

generations = 0

wordlist = []
fh = open("dictionary_english.dic")
ctr  =0 
for l in fh.readlines():
        l=l.strip().upper()
        if len(l)>=minwordlen and len(l)<=maxwordlen:
                wordlist.append(l)
                if ctr % 1000 == 0:
                        print str(ctr)+" words loaded, last was \""+l+"\""
                ctr += 1
wordlist = list(set(wordlist))
print str(ctr)+" words"


# hmm stringchars
#     N   D E BOTH
#     A A H O V
#     B B I P W
#     C C J Q X
#   A+B D K R Y
#   A+C E L S Z
#   B+C F M T .
# A+B+C G N U !
# ok so
# 
# 0 = 00000 no firing
# 1 = 00001 only ctrl
# 2 = 00010 only shift
# 3 = 00011 both
# 4 = 00100 C         C
# 5 = 00101 ctrl-c    Q
# 6 = 00110 shift-c   J
# 7 = 00111 both-c    X
# 8 = 01000 B         B
# 9 = 01001 ctrl-b    P
#10 = 01010 shift-b   I
#11 = 01011 both-b    W
#12 = 01100 b+c       F
#13 = 01101 ctrl-b+c  T
#14 = 01110 shift-b+c M
#15 = 01111 both-b+c  .
#16 = 10000 A         A
#17 = 10001 ctrl-a    O
#18 = 10010 shift-a   H
#19 = 10011 both-a    V
#20 = 10100 a+c       E
#21 = 10101 ctrl-a+c  S
#22 = 10110 shift-a+c L
#23 = 10111 both-a+c  Z     
#24 = 11000 a+b       D
#25 = 11001 ctrl-a+b  R
#26 = 11010 shift-a+b K
#27 = 11011 both-a+b  Y
#28 = 11100 a+b+c       G
#29 = 11101 ctrl-a+b+c  U
#30 = 11110 shift-a+b+c N
#31 = 11111 both-a+b+c  !


stringchars = [" "," "," "," ","C","Q","J","X","B","P","I","W","F","T","M",".","A","O","H","V","E","S","L","Z","D","R","K","Y","G","U","N","!"]

display = "0"*digits
no = 1
fh = ""
while no == 1:
    no = 0
    print "Load which net\n: ",
    fn = sys.stdin.readline().strip()
    
    try:
        fh = open(fn)
    except:
        print "Fail likely no such filename"
        no = 1
netfile = fh.readlines()
fh.close()


wireline = 0
wirefile = []
realline = 0
r = re.compile("[^-0-9 ]")
for l in netfile:
	l = r.sub("",l)
	if l.strip() != "" and type(l) == type("String"):
		wireline += 1
		wirefile.append(l.strip())
	if wireline == neurons:
		break
	realline += 1
print "wirefile:"
print repr(wirefile)

neurfile = []
# same r
ctr = realline+1
l = ""
while l.strip() == "":
	l=netfile[ctr]
	l=r.sub("",l)
	if l.strip() != "" and type(l) == type("String"):
		l=l.strip().split(" ")
		for a in l:
			neurfile.append(a)
		l=" ".join(l)
	ctr+=1

print "neurfile:"
print repr(neurfile)

wireprop = []
ctr = 0
for a in xrange(neurons):
	wireprop.append(" "*neurons)
	for b in xrange(neurons):
		wp = list(wireprop[ctr].strip())
		wp.insert(neurons-(ctr+1),wirefile[ctr])
	print "wp: "+repr(wp)
	wireprop.pop(); wireprop.append(wp)
	ctr += 1
	tl = []
	for n in wp[0].split(" "):
		try:
			tl.append(int(n))
		except:
			tl.append(-100)
	wireprop.pop(); wireprop.append(tl) # good

stringoutput = ""
neuronprop = []
ctr = 0
for a in xrange(neurons):
	neuronprop.append("0:"+neurfile[ctr])
	ctr += 1

print fn+" contents"
for l in netfile:
	print "\t"+l

print "neuron props"
print repr(neuronprop)

print "wire props"
print repr(wireprop)

print """Then I have the neuron properties and wire weights. I 
should still be able to change the neuron count dynamically if
the savefile is correct (contents-wise) too. Nice well brb\n"""


parentscore = 0 # christ

# Functions start

ucalph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
lcalph = "abcdefghijklmnopqrstuvwxyz"

def mainmenu():
	print ":Exhaustive Neural Optimization Regime Controller Front-End:"

	columns=[] # for 37 column output

	columns.append(":Wiring Map") # i figured it didnt matter if the whole wiring plan was backwards so it is, in this

	columns[-1] += "\n   "
	ctr = 0
	for n in list(ucalph):
                columns[-1]+=n+"   " # two spaces for low neuron counts like 5
		ctr += 1
		if ctr == neurons:
			break
	columns[-1] += "\n"
	ctr = 0
	for n in list(ucalph):
		columns[-1]+=" "+n+" "
		for c in wireprop[ctr]:
                        columns[-1]+=str(c).zfill(2)+"  "
		ctr+=1
		columns[-1]+="\n"
		if ctr == neurons:
			break

        columns.append("          :Neuron properties")
	columns[-1]+="\n Neur   Pot   Thresh"

	ctr =0 
	for n in list(ucalph):
		columns[-1]+="\n  "+n+"     "+neuronprop[ctr].split(":")[0]+"      "+neuronprop[ctr].split(":")[1]
		ctr += 1
		if ctr == neurons:
			break

        columns.append(":Commands 1\nquit      quit\nmutate    order a single mutation\nallmuts   list all valid muts\neval1     one frame neural time\nevaln     eval all frames til death\nhipot     set all pots to fire\nscore     score output at game\nclear     clear stringoutput\nstep      go to best mutant net 1 mut away\n2step     go to best net 2 muts away\nrand      try to find random good multiple muts\nmask      switch neuron/wire muts on/off\n")
        columns.append("                        :Calc display\n"+"                        ["+display+"]\n\n      :Parent score\n       "+str(parentscore)+"\n\n     : Mask\n        ["+mask+"]\n") # whats with these six spaces every right-side-margin tab




# columns ready

	ctr = 0
	maxlen = 0
	for c in xrange(len(columns)):
		if c&1:
			continue
		#try:
		maxlen = len(columns[c].split("\n"))
		maxlen2 = len(columns[c+1].split("\n"))
		if maxlen2 > maxlen:	maxlen = maxlen2
		linenum = 0
		for cc in xrange(maxlen):
			l = ""
			try:
				l += 	columns[c].split("\n")[cc]		
			except:
				l += " "*36
			l += "    "
			try:
				l += 	columns[c+1].split("\n")[cc]+"\n"
			except:
				l += "\n"
			print l,
			linenum += 1
		

def isavalidmutation(mut):
        leadingChar = mut[0];
        if leadingChar == 'n':
                try:
                        neur = lcalph.index(mut[1])
                except:
    #                    print "isavalidmutation: Non-alphabetic neuron"
                        return 0
                if neur >= neurons:
     #                   print "isavalidmutation: Neuron "+mut[1]+" (#"+str(neur)+") >= neurons ("+str(neurons)+") (no such neuron)"
                        return 0
                try:
                        thresh = int(mut[2:])
                except:
      #                  print "isavalidmutation: Non-numeric threshold"
                        return 0
                if thresh < minthresh:
       #                 print "isavalidmutation: New threshold less than minthresh"
                        return 0
                elif thresh > maxthresh:
        #                print "isavalidmutation: New threshold higher than maxthresh"
                        return 0
                if thresh == int(neuronprop[neur].split(":")[1]):
         #               print "isavalidmutation: Mutation would have no effect"
                        return 0
          #      print "isavalidmutation: Old threshold "+str(neuronprop[neur].split(":")[1])+", new threshold "+str(thresh)
                return 1
        elif leadingChar == 's':
                try:
                        neur1 = lcalph.index(mut[1])
                except:
           #             print "isavalidmutation: Non-alphabetic neuron"
                        return 0
                if neur1 >= neurons:
            #            print "isavalidmutation: Source neuron "+mut[1]+" (#"+str(neur1)+") >= neurons ("+str(neurons)+") (no such neuron)"
                        return 0
                try:
                        neur2 = lcalph.index(mut[2])
                except:
             #           print "isavalidmutation: Non-alphabetic neuron"
                        return 0
                if neur2 >= neurons:
              #          print "isavalidmutation: Target neuron "+mut[2]+" (#"+str(neur2)+") >= neurons ("+str(neurons)+") (no such neuron)"
                        return 0

                try:
                        weight = int(mut[3:])
                except:
               #         print "isavalidmutation: Non-numeric weight"
                        return 0
                if weight < minweight:
                #        print "isavalidmutation: New weight less than minweight"
                        return 0
                elif weight > maxweight:
                 #       print "isavalidmutation: New weight higher than maxweight"
                        return 0
                if weight == wireprop[neur2][neur1]:
                  #      print "isavalidmutation: Mutation would have no effect"
                        return 0
#                print "isavalidmutation: Old weight "+str(wireprop[neur2][neur1])+", new weight "+str(weight)
                return 1


        else:
                return 0

def mutate(mut):
        for mutt in mut.split(","):
                leadingChar = mutt[0]
                if leadingChar == 'n':
                        neur = lcalph.index(mutt[1])
                        thresh = int(mutt[2:])
                        (op,ot) = neuronprop[neur].split(":")
                        neuronprop[neur] = "%s%c%d" % (op, ':', thresh)
                        
                elif leadingChar == 's':
                        neur1 = lcalph.index(mutt[1])
                        neur2 = lcalph.index(mutt[2])
                        weight = int(mutt[3:])
                        wireprop[neur2][neur1] = int(weight)

def listallmuts():
        muts = []
        if mask.count("n"):
                for n in xrange(neurons):
                        for t in xrange(minthresh,maxthresh+1):
                                muts.append("%s%s%s" % ('n',lcalph[n],str(t)) )
                                if not isavalidmutation(muts[-1]): muts.pop()  # in case mutation has no effect
        if mask.count("s"):
                for s1 in xrange(neurons):
                        for s2 in xrange(neurons):
                                for w in xrange(minweight,maxweight+1):
                                        muts.append("%c%s%s%d" % ('s',lcalph[s1],lcalph[s2],w) )
                                        if not isavalidmutation(muts[-1]): muts.pop()  #  in case mutation has no effect
        return muts


def evaloneframeneuraltime():
        didfire = [0] * neurons
        global stringoutput
        num_fired = 0

        # first i want a list of which neurons fired
        for nidx in xrange(neurons):
                (pot, thresh) = neuronprop[nidx].split(":")
                pot = int(pot)
                thresh = int(thresh)
                if pot >= thresh:
                        didfire[nidx] = pot/thresh
                        neuronprop[nidx] = str(pot%thresh)+":"+str(thresh)
                        num_fired += 1

        # string production start
        mostfires = 0
        for n in didfire:
                if n > mostfires:
                        mostfires = n
                
        for c in xrange(mostfires):
                addr = 0
                pow = 16
                for n in didfire:
                        if n > c:
                                addr += pow
                        pow /= 2
                stringoutput += stringchars[addr]
        # string production end

        for nidx in xrange(neurons):
                (pot, thresh) = neuronprop[nidx].split(":")
                # check every neuron
                pot = int(pot)
                thresh = int(thresh)
                for sidx in xrange(neurons):
                        # check every synapse
                        weight=wireprop[nidx][sidx]
                        if didfire[sidx]>0:
                                pot += weight*didfire[sidx]
                                if pot <0: pot = 0
                neuronprop[nidx] = str(pot)+":"+str(thresh)

#        print "evaloneframeneuraltime: stringoutput: "+str(len(stringoutput))+" chars"

        return num_fired

def score(strn):
        mat = ""
        lpoints = 0
        for w in wordlist:
#                if len(w) >= minwordlen:
#                        w = " "+w+" "
                        count = stringoutput.count(w)
                        if count > 0:
#                                print "stringoutput contains \""+w+"\""
#                                print w+", ",
                                mat += w+", "
#                                sys.stdout.flush()
                                lpoints += ((len(w)*len(w))*100) * count
        return (lpoints, mat)

#def clear(): # clear stringoutput
#        stringoutput = ""

def evaln(max=0): # max = maximum output size
        global stringoutput
        did_fire = evaloneframeneuraltime()
        frames = 0
        while did_fire > 0:
                did_fire = evaloneframeneuraltime()
                frames += 1
                if max > 0 and len(stringoutput) >= max:
                        return frames

#    #    stringoutput = stringoutput[:maxoutputsize] # should keep it pared (trim)
        
        return frames


def hipot():
                for nidx in xrange(neurons):
                        (pot,thresh) = neuronprop[nidx].split(":")
#                        pot = str(int(pot)+maxthresh)
                        pot = str(maxthresh)
                        neuronprop[nidx] = pot+":"+thresh
        

def f(lis):
        tl=[]
        for item in lis:
                if type(item) == type([]):
                        item = []
                        for n in item:
                                item.append(n)                        
                tl.append(item)
        return tl

# Functions End


# interpreter loop
while 1:
        print;print
	mainmenu() # you dont pass this a neural net
     		      # it reads out the main neural net

        print ": ",
	cmd = sys.stdin.readline().strip().lower()

        if cmd == "allmuts":
                x = listallmuts()
                print
                print "List of all valid mutations from listallmuts():"
                print repr(x)
                print "Number of mutations "+str(len(x))
                print 

        elif cmd == "mutate":
                print "Interactive mutation. Enter n[A-E][2-12] to alter neuron threshold..."
                print "Enter s[A-E][A-E][-5-5] to alter synapse weight. eg nA9 or sAB2."
                print "When using s, source neuron comes first, target second."
                print ": ",
                mut = sys.stdin.readline().strip().lower()
                for mutt in mut.split(","):
                        if isavalidmutation(mutt):
                                print "Mutating ("+mutt+")"
                                mutate(mutt)
                        else:
                                print "Mutation string \""+mutt+"\" invalid."
        elif cmd == "mask":
                print "If mask contains 'n', mutate neurons. If mask contains 's', mutate wires."
                print "Mask: \""+mask+"\""
                print "Enter new mask, or leave blank to cancel."
                print ": ",
                oldmask = mask
                mask = sys.stdin.readline().strip().lower()
                if mask == "":
                        print "Cancelled"
                        mask = oldmask
                else:
                        print "New mask: \""+mask+"\""
        elif cmd == "quit":
		print "Exiting interactive mode"
		sys.exit(1)
        elif cmd == "eval1":
                print "Evaluating one frame of neural time"
                evaloneframeneuraltime()
                print "evaloneframeneuraltime: stringoutput: "+stringoutput

        elif cmd == "evaln":
                print "Evaluating n frames of time (until firing ceases)"
                frames=evaln(max=maxoutputsize)
                print "Evaluated "+str(frames)+" frames neural time til braindeath. :D"
        elif cmd == "hipot":
                print "Setting all pots to pot+maxthresh+1"
                hipot()
        elif cmd == "score":
                print "Stringoutput:"
                print "\""+stringoutput+"\""
                print "Score of stringoutput value:"
                (points,matches)=score(stringoutput)
                # why is matches here, smaller than bestsmatches i get if i
                # go hipot, evaln, score? hmm wonder if i have to go clear
                print "stringoutput score: "+str(points)
                print "value of matches:(debug) "+matches
                if parentscore != points:
                        print "parentscore set to "+str(points)
                        parentscore = points
        elif cmd == "clear":
                print "Clearing stringoutput."
                stringoutput=""
        elif cmd == "step" or cmd == "2step":
                if cmd == "2step":
                        print "Finding best double-mutation-mutant net."
                        mutstotry = listallmuts()
                        tl = list(mutstotry)
                        for idx in xrange(len(tl)):
                                for m in tl:
                                        mutstotry.append(mutstotry[idx]+","+m)
                elif cmd == "step":
                        print "Finding best single-mutation-mutant net."
                        mutstotry = listallmuts()
                print str(len(mutstotry)) + " mutant nets to try ..."

                parentneurs = list(neuronprop)
                parentwires = []
                for nn in wireprop:
                        parentwires.append(list(nn))
                midx = 0
                hiscore = parentscore
#                bestneurs = []
#                bestwires = []
                bestmut = ""
                bestidx = 0
                bestoutput = ""
                bestsmatches = ""
                for mut in mutstotry:
                        points = 0
                        wireprop = []#list(parentwires)
                        for nn in parentwires:
                                wireprop.append(list(nn)) # fucks sake lists of lists get fucked up

                        neuronprop = list(parentneurs)
                        mutate(mut)

#                        print parentwires # debug
#                        mainmenu() # debug


                        print "debug: mutation \""+mut+"\""
                        stringoutput=""
                        hipot()
                        print "Mutant #"+str(midx)+" ran for "+str(evaln(max=maxoutputsize))+" frames."
                        print "Scoring mutant #"+str(midx)+"'s output, last line:\n\""+stringoutput[-76:]+"\""
                        (points,matches) =score(stringoutput)
                        print "debug: mutant #"+str(midx)+"'s score: "+str(points)
                        if points > hiscore:
 #                               bestneurs = []
#                                bestwires = []
  #                              bestneurs.extend(neuronprop)
   #                             bestwires.extend(wireprop)
                                print "Mutant #"+str(midx)+" is the new high scorer."
                                hiscore = int(points  )
                                bestidx = int(midx)
                                bestmut = str(mut)
                                bestoutput = str(stringoutput)
                                bestsmatches=matches
                        midx += 1
                        time.sleep(delays)  # debug
                if hiscore <= parentscore:
                        neuronprop = []
                        neuronprop=list(parentneurs)
                        wireprop = []
                        wireprop=list(parentwires)

                        print "***Local optima! No mutant better than parent found."
                        print "\n\n***Evolution receipt***"
                        print str(generations)+" succesful steps accumulated. (Store lineages, flamoot!)"
                        print "***"
                else:
                        neuronprop = []
                        neuronprop=list(parentneurs)
                        wireprop = []
                        wireprop=list(parentwires)
                        mutate(bestmut)
                        parentscore = hiscore
                        generations += 1
                        print "\n\n***Evolution receipt***"
                        print str(generations)+" succesful steps accumulated. (Store lineages, flamoot!)"
                        stringoutput = bestoutput
                        print "Setting new parent to high scorer, mutant #"+str(bestidx)+" with score "+str(hiscore)+".\nOutput: "+bestoutput+"\nMatching: "+bestsmatches+"\nMutation: \""+bestmut+"\""
                        print "***\n"



        elif cmd == "rand":
                print "Finding best mutant net of 1000 random multiple-mutants."
                mutstotry = listallmuts()
                tmuts = []
                for nn in xrange(1000):
                        mutt = ""
                        depth = random.choice(range(maxranddepth))+2
                        for nnn in xrange(depth):
                                mutt += random.choice(mutstotry)+","
                        mutt = mutt.strip(",") 
                        tmuts.append(mutt)

                mutstotry = []
                mutstotry.extend(tmuts)
                print str(len(mutstotry)) + " mutant nets to try ..."
                parentneurs = list(neuronprop)
                parentwires = []
                for nn in wireprop:
                        parentwires.append(list(nn))
                midx = 0
                hiscore = parentscore
#                bestneurs = []
#                bestwires = []
                bestmut = ""
                bestidx = 0
                bestoutput = ""
                bestsmatches = ""
                for mut in mutstotry:
                        points = 0
                        wireprop = []#list(parentwires)
                        for nn in parentwires:
                                wireprop.append(list(nn)) # fucks sake lists of lists get fucked up

                        neuronprop = list(parentneurs)
                        mutate(mut)

#                        print parentwires # debug
#                        mainmenu() # debug


                        print "debug: mutation \""+mut+"\""
                        stringoutput=""
                        hipot()
                        print "Mutant #"+str(midx)+" ran for "+str(evaln(max=maxoutputsize))+" frames."
                        print "Scoring mutant #"+str(midx)+"'s output, last line:\n\""+stringoutput[-76:]+"\""
                        (points,matches) =score(stringoutput)
                        print "debug: mutant #"+str(midx)+"'s score: "+str(points)
                        if points > hiscore:
 #                               bestneurs = []
#                                bestwires = []
  #                              bestneurs.extend(neuronprop)
   #                             bestwires.extend(wireprop)
                                print "Mutant #"+str(midx)+" is the new high scorer."
                                hiscore = int(points  )
                                bestidx = int(midx)
                                bestmut = str(mut)
                                bestoutput = str(stringoutput)
                                bestsmatches=matches
                        midx += 1
                        time.sleep(delays)  # debug
                if hiscore <= parentscore:
                        print "***Local optima! No mutant better than parent found."
                        print "\n\n***Evolution receipt***"
                        print str(generations)+" succesful steps accumulated. (Store lineages, flamoot!)"
                        print "***"
                        neuronprop = []
                        neuronprop=list(parentneurs)
                        wireprop = []
                        wireprop=list(parentwires)

                else:
                        neuronprop = []
                        neuronprop=list(parentneurs)
                        wireprop = []
                        wireprop=list(parentwires)
                        mutate(bestmut)
                        parentscore = hiscore
                        generations += 1
                        print "\n\n***Evolution receipt***"
                        print str(generations)+" succesful steps accumulated. (Store lineages, flamoot!)"
                        stringoutput = bestoutput
                        print "Setting new parent to high scorer, mutant #"+str(bestidx)+" with score "+str(hiscore)+".\nOutput: "+bestoutput+"\nMatching: "+bestsmatches+"\nMutation: \""+bestmut+"\""
                        print "***\n"


	else:
		print "*** No such cmd, cycling\n"


# thats it for now.
