# Ants
It's an esolang.

But how do I use ANTS?

# The Basics

It's quite simple! Ants treats the input file as a 2D grid.

You set ants on the grid with colons, and move them by using pheromones, represented as periods.
Ants will move to the nearest available pheromone, executing any commands in their path.

# Walls and Gates

You can prevent ants from sensing pheromones by using walls and gates!

### Walls
Walls are represented by pipes and dashes. `{  - |  }`

If a wall is above an ant, it will not be able to sense pheromones any higher than that wall is.
This is true for all directions.

### Gates

Gates do the same as walls, except only from one side.

Gates are represented by four characters, the up arrow, left arrow, right arrow, and tilde. `{  ^ < > ~  }`

The direction the arrow is facing is the direction from which the ant can sense the pheromones. (For the tilde the direction is down.)

For example, if there is a left arrow to the left of an ant, (` <  : `), the ant can sense pheromones from beyond that gate to the left. 
Once that gate is to the right of the ant however, it can no longer sense pheromones from the right beyond that gate.

# The Stack
Ants, like other esolangs, features a stack.
The stack is an integer stack, and can be modified with commands.
When adding to the stack, items are added on top, and taken from the top.


# Commands
All commands are represented by a single character, and can take in arguments.

To represent the end of a command, you use a backtick ( ` ).

All arguments go between the command character and the backtick, and are seperated by semicolons.

For example, to print something to the console, you would use code like this:
```ants
:   pHello world!`   .
```

A very important command is `q`.

If an ant never passes the `q` command, the program never ends. As you might have guessed, `q` exits the program. To refactor our code, it would look more like this:
```ants
:    pHello world!`      q.
```
`q` does not require to be followed by a backtick.
Another command that does not require a backtick following it is `z`.

`z` checks if the top value in the stack is equal to 0. If so, it tells the current ant to check for any new pheromones, otherwise it does nothing.

Below is a list of commands, their arguments, and what they do.
### Command List
- `p<args...>` : Prints out all passed in args.
- `l<args...>` : Prints out all passed in args and a new line.
- `P` : Pops the top value of the stack off and prints it.
- `L` : Pops the top value of the stack off and prints it and a new line.
- `b` : Duplicates the top value of the stack.
- `i` : Takes user input and pushes it on to the stack.
- `a` : Removes the top two values from the stack and pushes the sum on to the stack.
- `s` : Pops `x` and `y` off the stack and pushes `x` - `y` on to the stack.
- `m` : Removes the top two values from the stack and pushes the product on to the stack.
- `d` : Pops `x` and `y` off the stack and pushes `x` / `y` on to the stack.
- `t<number>` : Pushes `number` on to the stack.

