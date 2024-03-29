#~~~~~~~~~~~~~~~~~~
## Description ####
#~~~~~~~~~~~~~~~~~~

# This is step 3 of 3 for my own "Texel's Tuning Method"

# Here a logistic regression is performed to find the optimal PST for middle and endgame
# The objective is to predict the outcome of the games

#~~~~~~~~~~~~~~
## Imports ####
#~~~~~~~~~~~~~~

library(rchess)
library(devtools)
setwd("PurplePanda/Texel")
source_url("https://raw.githubusercontent.com/Jakob256/UsefulStuff/main/UsefulStuff.R")
boards=readRDS("boards.rds")
results=readRDS("results.rds")



#~~~~~~~~~~~~~~~~
## Functions ####
#~~~~~~~~~~~~~~~~

sigmoid <- function(a,k=1){return(1/(1+exp(-a*k)))}

predError<- function(pred1,pred2){
  return(sqrt(sum((pred1-pred2)^2)/length(pred1)))
}

gamePhase24 <- function(board){
  return(sum(abs(board)==2)*2+
           sum(abs(board)==3)*1+
           sum(abs(board)==4)*1+
           sum(abs(board)==5)*4)
}

whichSquare <- function(number,adInfo="",adInfo2=""){
  what=c("Pawn_mg","Rook_mg","Knight_mg","Bishop_mg","Queen_mg","King_mg",
         "Pawn_eg","Rook_eg","Knight_eg","Bishop_eg","Queen_eg","King_eg")
  for (i in 1:12){
    if (number>64*(i-1)  & number <=64*i){cat(number,adInfo,adInfo2,what[i],": ")}
  }
  
  number=number%%64
  if (number==0){number=64}
  m=matrix(0,8,8)
  m[number]=1
  cat(letters[which(colSums(m)==1)])
  cat(9-which(rowSums(m)==1))
  
  cat("\n")
}

invalidPawns0 <- function(this){
  this[seq(8,64,8)]=0
  this[seq(1,64,8)]=0
  this[seq(8+384,64+384,8)]=0
  this[seq(1+384,64+384,8)]=0
  return(this)
}
 
exportPST <- function(this){
  
  offset=c(1,5,3,3,9,0,1,5,3,3,9,0)
  for (p in 1:12){
    
    #table=matrix(best[((p-1)*64+1):(64*p)],8,8)-offset[p]
    #table=matrix(a$par[((p-1)*64+1):(64*p)],8,8)-offset[p]
    table=matrix(this[((p-1)*64+1):(64*p)],8,8)-offset[p]
    
    table=round(table*100,2)
    
    cat("\n\n")
    if (p== 1){cat("int PST_Pawn_mg[8][8]={\n")}
    if (p== 2){cat("int PST_Rook_mg[8][8]={\n")}
    if (p== 3){cat("int PST_Knight_mg[8][8]={\n")}
    if (p== 4){cat("int PST_Bishop_mg[8][8]={\n")}
    if (p== 5){cat("int PST_Queen_mg[8][8]={\n")}
    if (p== 6){cat("int PST_King_mg[8][8]={\n")}
    if (p== 7){cat("int PST_Pawn_eg[8][8]={\n")}
    if (p== 8){cat("int PST_Rook_eg[8][8]={\n")}
    if (p== 9){cat("int PST_Knight_eg[8][8]={\n")}
    if (p==10){cat("int PST_Bishop_eg[8][8]={\n")}
    if (p==11){cat("int PST_Queen_eg[8][8]={\n")}
    if (p==12){cat("int PST_King_eg[8][8]={\n")}
    
    
    for (i in 1:8){
      cat("{")
      for (j in 1:8){
        cat(round(table[i,j]))
        if (j<8){cat(",")}
      }
      if (i!=8){cat("},\n")}
      if (i==8){cat("}};\n")}
    }
    
  }
}

 
#~~~~~~~~~~~~~~~~~~~~~~~~~~
## Preparing hugeTable ####
#~~~~~~~~~~~~~~~~~~~~~~~~~~



hugeTable=matrix(0,nrow=length(boards),ncol=64*6*2)



for (i in 1:length(boards)){
  .progress(i,length(boards))
  
  board=boards[[i]]
  gamePhase=min(gamePhase24(board),24)
  
  
  for (j in 1:768){
    type=ceiling(j/64)
    type=(type-1)%%6+1
    #print(type)
    pos=(j-1)%%64+1
    if (board[pos]==type){hugeTable[i,j]=hugeTable[i,j]+1}
    if (board[8:1,][pos]==-type){hugeTable[i,j]=hugeTable[i,j]-1}
  }
  
  ## gamephase update
  
  hugeTable[i,1:384]=hugeTable[i,1:384]*gamePhase/24
  hugeTable[i,385:768]=hugeTable[i,385:768]*(1-gamePhase/24)
}

#saveRDS(hugeTable, file = "hugeTable.rds")
hugeTable=readRDS("hugeTable.rds")

rm(i,j,pos,type,gamePhase)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## Updating 12 variables ####
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~


this_short=c(1,5,3,3,10,0,1,5,3,3,9,0)
this=rep(this_short,each=64)
thisEval=colSums(t(hugeTable)*this)
thisScore=predError(sigmoid(thisEval,0.35),results)

bestScore=thisScore
best=this_short

#stepSize=1.28*2
for (iteration in 1:10){
  stop("This loop is not used")
  
  #stepSize=stepSize*0.5
  stepSize=0.1
  #if (stepSize<0.006){break}
  
  for (k in c(1:5,7:11)){
    for (i in c(-stepSize,stepSize)){
      this_short=best
      this_short[k]=this_short[k]+i
      print(round(this_short,2))
      this=rep(this_short,each=64)
      thisEval=colSums(t(hugeTable)*this)
      thisScore=predError(sigmoid(thisEval,0.35),results)
      
      cat(iteration,k,thisScore,"\n")
      if (thisScore<bestScore){
        bestScore=thisScore
        best=this_short
        cat("yay\n")
        break ## pruning
      }
      
      
    }
  }
}



## setting invalid Pawns to "0"
this=invalidPawns0(this)

rm(best,bestScore,gamePhase,i,iteration,j,k,pos,stepSize,this_short,thisScore,type,thisEval)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## Updating 768 variables ####
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



this_short=c(1,5,3,3,10,0,1,5,3,3,9,0)
#this_short=round(this_short/0.03)*0.03 <- Texel 10 try
this=rep(this_short,each=64)

thisEval=colSums(t(hugeTable)*this)
thisScore=predError(sigmoid(thisEval,0.35),results)
start=this

n=30
for (iteration in 1:(2*n)){
  updates=0
  stepSize=0.03
  #if (iteration==n+1){stepSize=0.01}
  
  for (k in 1:768){

    
    ## pruning:
    isPawn=(k<=64) | (385 <=k && k<=448)
    if (isPawn & (k%%8) %in% c(0,1)){next}    ## pawns in row 1 and 8
    
    ## give pawns more time to update:
    if (iteration>n & !isPawn){next}
    if (iteration>n & !(k%%8 %in% c(2,3))){next}
    
    ## guessing if the value will be increased or decreased
    guess=stepSize
    if (start[k]>this[k]){
      guess=-stepSize
    }
    
    for (step in c(guess,-guess)){
      if (this[k]+step<0.8  && isPawn){next} ## no pawn should be less than 0.8
      
      newEval=thisEval+hugeTable[,k]*step
      prediction=sigmoid(newEval,0.35)
      newScore=predError(prediction,results)
      
      if (newScore<thisScore){
        thisScore=newScore
        this[k]=this[k]+step
        thisEval=newEval
        updates=updates+1
        break ## pruning
      }
      
      ## good guess or bad guess printing was wrong...
      cat(iteration,stepSize,k,updates,thisScore,"\n")
      
    }
  }
  
  
  
}

#file=paste("this",n,".rds",sep="")
#saveRDS(this, file)

exportPST(this)
