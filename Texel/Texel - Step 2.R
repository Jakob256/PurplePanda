#~~~~~~~~~~~~~~~~~~
## Description ####
#~~~~~~~~~~~~~~~~~~

# This is step 2 of 3 for my own "Texel's Tuning Method"

# Here we will extract from each game in games.rds one board position.
# The only rule is, that this position can't be followed by a capture.
# This board positions will be safed in boards.rds
# Takes 4 hours or so

#~~~~~~~~~~~~~~
## Imports ####
#~~~~~~~~~~~~~~

setwd("PurplePanda/Texel")
library(rchess)
gamesLan=readRDS("gamesLan.rds")
results=readRDS("results.rds")
length(gamesLan)
length(results)


#~~~~~~~~~~~~~~~~
## Functions ####
#~~~~~~~~~~~~~~~~


nrMoves <- function(lan){return(length(unlist(strsplit(lan," "))))}

subGame <- function(lan,n){return(paste0(unlist(strsplit(lan," "))[1:n],collapse = " "))}

lan2board <- function(lan){
  inp=rep("",length(lan)*2+1)
  inp[seq(from=1,by=2,length.out=length(lan))]=paste("position startpos moves",lan)
  inp[seq(from=2,by=2,length.out=length(lan))]="d"
  inp[length(inp)]="quit"
  
  cat("\nconsulting PurplePanda10.exe ... ")
  a=system2("PurplePanda10.exe",input=inp,invisible = F,stdout=T)
  cat("finished!!!\n\n")
  a=a[substr(a,1,1)=="|"]
  
  stopifnot(length(a)==length(lan)*8)
  
  
  res=list()
  
  for (k in 1:length(lan)){
    if (k<100 | k%%1000==0){cat("\r",k)}
    m=matrix(0,8,8)
    for (i in 1:8){
      for (j in 1:8){
        what=substr(a[(k-1)*8+i],4*j-1,4*j-1)
        if (what=="k"){m[i,j]=-6}
        if (what=="q"){m[i,j]=-5}
        if (what=="b"){m[i,j]=-4}
        if (what=="n"){m[i,j]=-3}
        if (what=="r"){m[i,j]=-2}
        if (what=="p"){m[i,j]=-1}
        if (what=="P"){m[i,j]=+1}
        if (what=="R"){m[i,j]=+2}
        if (what=="N"){m[i,j]=+3}
        if (what=="B"){m[i,j]=+4}
        if (what=="Q"){m[i,j]=+5}
        if (what=="K"){m[i,j]=+6}
      }
    }
    res[[k]]=m
  }
  cat("\ndone\n")
  return(res)
}

nrPieces <- function(board){return(sum(board!=0))}


#~~~~~~~~~~~~~~~
## Let's go ####
#~~~~~~~~~~~~~~~


movesCutoff=rep(NA,length(gamesLan))
subGames1=rep(NA,length(gamesLan))
subGames2=rep(NA,length(gamesLan))
subgameOk=rep(FALSE,length(gamesLan))


for (round in 1:1000){
  cat("round",round,"\n\n")
  
  
  for (i in which(!subgameOk)){
    if (i%%1000==0){cat("\r",i)}
    
    game=gamesLan[i]
    
    movesCutoff[i]=sample(1:(nrMoves(game)-1),1)
    subGames1[i]=subGame(game,movesCutoff[i])
    subGames2[i]=subGame(game,movesCutoff[i]+1)
  }
  
  
  nrPieces1=unlist(lapply(lan2board(subGames1[!subgameOk]), nrPieces))
  nrPieces2=unlist(lapply(lan2board(subGames2[!subgameOk]), nrPieces))
  
  subgameOk[!subgameOk]=(nrPieces1==nrPieces2)
  print(sum(nrPieces1==nrPieces2))
  
  if (all(nrPieces1==nrPieces2)){break}
}




boards=lan2board(subGames1)
saveRDS(boards, file = "boards.rds")


## final verification:
nrPieces1=unlist(lapply(boards              , nrPieces))
nrPieces2=unlist(lapply(lan2board(subGames2), nrPieces))
all(nrPieces1==nrPieces2)
