generalizedSpread2 <- function(x, o) {
  front <- paretoFront(x)
  truefront <- paretoFront(o)

  ## Normalize front:
  maxval <- apply(truefront, 2, max)
  minval <- apply(truefront, 2, min)

  nfront <- normalizeFront(front, minval, maxval)
  ntruefront <- normalizeFront(truefront, minval, maxval)

  K <- nrow(nfront)
  N <- nrow(ntruefront)
  ## Calculate extreme values:
  nobj <- ncol(front)

##  extreme <- matrix(0, ncol=nobj, nrow=nobj)
##  for (i in 1:nobj) {
##    o <- order(ntruefront[,i])
##    for (j in 1:nobj) {
##      extreme[i,j] <- ntruefront[o,][N, j]
##    }
##  }
  extreme <- sapply(1:nobj, function(i) ntruefront[which.max(ntruefront[,i]),])
  ## Lexographically sort front:
  for (i in nobj:1)
    nfront <- nfront[order(nfront[,i]),]

  if (distance2(nfront[1,], nfront[K,]) == 0) {
    return (0.0)
  } else {
    dmean <- mean(sapply(1:K, function(i) sqrt(distanceToFront2(nfront[i,], nfront[-i,]))))
    dextr <- sum(sapply(1:nobj, function(i) sqrt(distanceToFront2(extreme[i,], nfront))))
    mean <-  sum(abs(sapply(1:K, function(i) sqrt(distanceToFront2(nfront[i,], nfront[-i,]))-dmean)))

    return ((dextr + mean)/(dextr + K*dmean))
  }
}




generalizedSpread2(data,Optimal)
