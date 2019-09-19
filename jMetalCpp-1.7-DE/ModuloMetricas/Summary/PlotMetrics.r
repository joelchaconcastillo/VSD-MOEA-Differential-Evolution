##Este script tiene reliza un gráfico de los resultados dada una métrica...
args<-commandArgs(TRUE)

BoundsXObjective= c(0,3)
BoundsYObjective = c(0,5)
ProblemName = args[1]
pdf(file = paste(ProblemName,".pdf", sep=""))
#GenerationalDistance =  read.table( paste(ProblemName,"_GenerationalDistance.txt", sep="" ))
#plot(GenerationalDistance , xlab="Generation", ylab="Metric", main= "Generational Distance", type="n")
#lines(GenerationalDistance)
#Spacing=  read.table( paste(ProblemName,"_Spacing.txt", sep="" ))
#plot(Spacing, xlab="Generation", ylab="Metric", main= "Spacing", type="n")
#lines(Spacing)
#
Objectives=  read.table( paste(ProblemName,"_ClosestDistanceDecisionVariables.txt", sep="" ))
plot(Objectives, xlab="Generation", ylab="Metric", main="Closest Distance Variables", type="n" )
lines(Objectives)

Objectives=  read.table( paste(ProblemName,"_Objectives.txt", sep="" ))
plot(Objectives, xlab="Generation", ylab="Metric", xlim=BoundsXObjective, ylim=BoundsYObjective, cex=0.2, main=ProblemName)
par(new = T)
OptimalParetoFront = paste("./../OptimalFronts/WFG_TwoObjectiveSolutions/", paste(ProblemName, ".txt",sep="") ,sep="");
#OptimalParetoFront = paste("./../OptimalFronts/", paste(ProblemName, ".txt",sep="") ,sep="");
data = read.table(OptimalParetoFront, sep=" ")
#x = data[,1]
#y = data[,2]
#Indexdesc = order(y)
#abline(a=x[Indexdesc], b=y[Indexdesc])

plot(data, col="red", xlim=BoundsXObjective, ylim=BoundsYObjective, cex=0.2, main=ProblemName)
par(new = T)
dev.off()
