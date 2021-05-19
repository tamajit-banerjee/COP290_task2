# 

A simple maze-based two-player game written in C++, using SDL and socket programming!

## Basic Outline of Game

As a player roams around in a randomly generated maze, each of the two players tries to collect coins to increase their score. As with life, their time in the game is limited, and a constant stop-watch tells them how long they have left. The players also have the option of eating a 'clock' that grants them extra time (thus extending their life).

The players cannot move freely in the maze though. Randomly located in the maze are four monsters! Two of the monsters chase the players (one each), and can randomly stop chasing them and again randomlty start chasing them. The players constantly need to take care that they are not eaten by a monster. We believe in second chances, and all players are given quite a few chances. On being hit by a monster, the player does not simply die, and merely loses some time (shortening their life).

As the players move about collecting coins, they have to carefully weigh their options. Being eaten by a monster is not the end of the world, so they may sometimes chose to go right through a monster. Again they have to decide whether to go towards coins or times.

## Some rules

## How to win

# Algorithmic Details

### The Player

### The monster

### The maze cell

