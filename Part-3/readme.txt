Our program contains the following scripts:


******** getApartments ********

# Recieves
  1) Text file containing sources from different realtor agencies.
  optional:
  2) The city to find apartments in. If absent, default will be Haifa.
  3) Number of apartments to be printed. If absent, default will be 1.

$ What it does
  Prints to stdout:
 * "Illegal or missing parameters" if number of parameters given isn't 1 or 3.
 * "File is missing" if sources file doesn't exist in current directory.
 * Otherwise, given number of available apartments in given city by order of:
     1) Price, by ascending order.

     2) Dates, in descending order.

     3) Order in original source.
     Identical apartments will be printed once.

******** sortFiles ********

# Recieves
  List of sources from stdin, each in a separate line.

$ What it does
  Prints to stdout the content of sources that ends with ".flat".
  By:
  Reads each line by loop and for each source:
  1) If source doesn't end with ".flat"- read next source.
  2) If source end with ".flat" and source is a file- prints the source's content to stdout.
  3) If source end with ".flat" and source is a directory- calls recuresively to "search" with all files in that directory.  

******** cleanLines ********

# Recieves
  lines of texts from stdin, for editing.

$ What it does
  Removes comments and empty lines from input and prints result to stdout,
  By:
  Reads lines by loop and for each line:
  1) Cuts the comments in the line, if exist.
      If not, nothing is done.
  2) Checks if edited line is not-empty.
      If so, echoes the line.
      If not, nothing is done.

******** handleNamesDates ********

# Recieves
  A text input containing lines of apartments and their info, for organizing.

$ What it does
  It goes through the lines in the text, searching for names made up of several
  words, switching the spaces between them into underlines (" " into "_").
  It also switches all the dates in the file into yyyymmdd format 
  (example: 30.9.1992 into 19920930) and adds the line's number at the start of it.
  All of this is done to make every line have the same amount of fields, to make
  the sorting easier and possible.

---- functions ----

** changeDate **

# Recieves
  A line of text from the text sent to handleNamesDates, for editing.

$ What it does
  It goes to the end of the line recieved, takes the date, adds zeroes to
  months and days that written with a single digit, if needed.
  After that it reads to the new date to the rest of the line, instead of
  the old date.

******** sortFile ********

# Recieves
  A list of lines of apartments, after they were treated by handleNamesDates,
  in order to sort those lines.

$ What it does
  First of all, it uses uniq and sort by the line's numbers added by handleNamesDates
  to remove all multiple appearances of identical apartments. 
  Then, it does the following actions (by this order):
  1) Sorts all the apartments according to their price (from lowest to highest)
  2) Gets rid of apartments that appear more than once
  3) Removes all but a number of apartments, depending on what 'limit' the user
     entered when executing getApartments.
  4) Sorts the apartments according to their dates, from most recent to oldest
  5) For every apartment left at this stage, it prints to the user the number
     of rooms, rent price and the telephone number.
  
  * After all these actions, if apartments with the same price and date exist, 
    their order will be in the order they were found in the files.