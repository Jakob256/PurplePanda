#~~~~~~~~~~~~~~~~~~
## Description ####
#~~~~~~~~~~~~~~~~~~

# This is step 1 of 3 for my own "Texel's Tuning Method"

# The dataset used is Lichess Elite Database.
# It can be found on https://database.nikonoel.fr/
# It is a collection of games played on lichess between strong players
# Specifically, I am using the dataset "lichess_elite_2022-01.pgn"
# SAN = Standard algebraic notation
# Takes 6 days or so

# Here, we do some datacleaning, and save the games in two files:
#   1) gamesLan.rds: The games in lan notation (i.e. "e2e4 d7d5 e4d5 ...")
#   2) results.rds: The results of the games. 1 for white win, 0.5 for draw, 0 for black


#~~~~~~~~~~~~~~
## Imports ####
#~~~~~~~~~~~~~~

setwd("PurplePanda/Texel")
library(rchess)

#~~~~~~~~~~~~~~~
## Let's go ####
#~~~~~~~~~~~~~~~

data=read.csv("lichess_elite_2022-01.pgn")[[1]]

data=data[substr(data,1,1)!="[" | substr(data,1,7)=="[Result"]

nr=sum(substr(data,1,7)=="[Result")

results=data[substr(data,1,7)=="[Result"]
games=rep(NA,nr)
filled=0
game=""

for (i in 1:length(data)){
  if (i%%100000==0){print(i)}
  line=data[i]
  if (substr(line,1,7)=="[Result"){
    
    if (i!=1){
      filled=filled+1
      games[filled]=game
    }
    game=""
    
      
  } else {
    game=paste0(game,line,sep=" ")
  }
}
filled=filled+1
games[filled]=game

if (FALSE){
  head(games,1)
  head(results,1)
  tail(games,1)
  tail(results,1)
  games[17]
  results[17]
}

keep=(nchar(games)>150) ## only use games with 13 moves from both sides
games=games[keep]
results=results[keep]
nr=sum(keep)
rm(keep)

games[1]
results[1]


for (i in 1:length(games)){
  game=games[i]
  if (results[i]=="[Result 1-0]"){game=substr(game,1,nchar(game)-4)}
  if (results[i]=="[Result 0-1]"){game=substr(game,1,nchar(game)-4)}
  if (results[i]=="[Result 1/2-1/2]"){game=substr(game,1,nchar(game)-8)}
  games[i]=game
}



gamesLan=rep("",length(games))


for (i in 1:length(games)){ ## done till 426.000
  cat(i,"of",length(games),"\n")
  gamesLan[i]=tolower(bigchess::san2lan(games[i]))
}


results[results=="[Result 1-0]"]=1
results[results=="[Result 0-1]"]=0
results[results=="[Result 1/2-1/2]"]=0.5
results=as.double(results)

if (sum(nchar(gamesLan)>10)==length(gamesLan)){
  saveRDS(gamesLan, file = "gamesLan.rds")
  saveRDS(results, file = "results.rds")
}

