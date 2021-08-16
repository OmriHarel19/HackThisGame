#!/usr/bin/env python2.7

import random
import sqlite3
import sys
import os

# printing vars:
BANNER = "=== Player Data Base ==="

MENU = """\
1) add score
2) view player scores
3) view score
4) shuffle player
5) exit"""


#print(os.getcwd())
# get the flag:
with open('flag.txt') as fd:
  flag = fd.read()

# establish connection
conn = sqlite3.connect(':memory:')
c = conn.cursor()

# create a special flag table:
c.execute("CREATE TABLE flag_table (flag text)")

# create the database table that contains players in rows of: {name, score}
c.execute("CREATE TABLE database (name text, score text)")
c.execute("INSERT INTO flag_table VALUES ('{}')".format(flag))

# functions:

# custom print to standart output (console)
def my_print(s):
  sys.stdout.write(s + '\n')
  sys.stdout.flush()

# execute the command provided in quary and print the outcome
def print_list(query):
  my_print("")
  my_print("== view ==")
  
  for i, res in enumerate(c.execute(query).fetchall()):
    my_print('{}: "{}" , "{}"'.format(i+1, res[1], res[0]))
  
  my_print("")


# program:
my_print(BANNER)

# menu loop:
while True:
  my_print(MENU)
  sys.stdout.write("> ")
  sys.stdout.flush()

  # get input
  choice = raw_input()
  #validate:
  if choice not in ['1', '2', '3', '4', '5']:
    my_print('invalid input')
    continue
  
  # choise 1: add score
  if choice == '1':
    # get player nickname
    my_print("nickname?")
    name = raw_input().replace('"', "") # avoid '"' character to prevent an injection
    #get player score
    my_print("score?")
    score = raw_input().replace('"', "") # avoid '"' character to prevent an injection
    # add to database
    c.execute("""INSERT INTO database VALUES ("{}", "{}")""".format(name, score))
  
  # choise 2: view player scores
  elif choice == '2':
    # get player nickname
    my_print("nickname?")
    name = raw_input().replace("'", "") # avoid ' character to prevent an injection
    # print player record
    print_list("SELECT name, score FROM database WHERE name = '{}'".format(name))

  # choise 3: view score
  elif choice == '3':
    # get score
    my_print("score?")
    score = raw_input().replace("'", "") # avoid ' character to prevent an injection
    # print all player that have the given score
    print_list("SELECT name, score FROM database WHERE score = '{}'".format(score))

  # choise 4: shuffle player at random
  elif choice == '4':
    # choose a random name from the data base
    name = random.choice(list(c.execute("SELECT DISTINCT name FROM database")))[0]
    my_print("choosing scores from random name: {}".format(name))
    # print random player
    print_list("SELECT name, score FROM database WHERE name = '{}'".format(name))
  
  # choise 5: exit
  else:
    my_print("bye")
    exit(0)
