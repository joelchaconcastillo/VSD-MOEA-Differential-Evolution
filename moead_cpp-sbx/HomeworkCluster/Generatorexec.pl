#Con este script se genera la lista de ejecuciones....
#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
my $file = "ExecutionFileDiversity";
my $fout;
open($fout, '>' ,$file);
my $Path =  `cd ..; pwd`;#"/home/joel.chacon/Current/MyResearchTopics/MOEA-D-Diversity/MOEAD-DE/vsd-moead-opt";
chomp $Path;

####Realizar la búsqueda del parámetro D inicial que proporcione mejores resultados
my $PathAlgorithm = $Path;

my @Instance = ("DTLZ1", "DTLZ2", "DTLZ3", "DTLZ4", "DTLZ5", "DTLZ6", "DTLZ7");
my $prob = 0.9;
my $neighboursize = 20;
my $pops=100;
my $max_nfes=250000*$pops;
my $limit=2;

#for(my $t=1; $t<=10; $t++)
#{
#$neighboursize=$t*10;
   foreach(@Instance)
   {
   	my $nvar;
   	
   	for(my $nobj = 2; $nobj <=3; $nobj++)
   	{
   
      	   if($_ eq "DTLZ1")
   	   {
   	      $nvar=5+$nobj-1;
   	   }
   	   elsif($_ eq "DTLZ7")
   	   {
   	      $nvar=20+$nobj-1;
   	   }
   	   else
   	   {
   	      $nvar=10+$nobj-1;
   	   }
   	
   	   for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
   	   {
   	   	print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes $neighboursize $prob $limit $nvar\n";
   	   }
   	}
   }
   #}	


@Instance = ("WFG1", "WFG2", "WFG3", "WFG4", "WFG5", "WFG6", "WFG7", "WFG8", "WFG9");
#for(my $t=1; $t<=10; $t++)
#{
#  $neighboursize=$t*10;
   foreach(@Instance)
   {
   	my $nvar;
   	
   	for(my $nobj = 2; $nobj <=3; $nobj++)
   	{
   	   my $k =4;# 2*($nobj-1);
   	   my $l =20;# 24-$k;
   	   $nvar=$l+$k;
   	
   	   for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
   	   {
   	   	print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes $neighboursize $prob $limit $l $k\n";
		#   	   	print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes  $neighboursize $prob $l $k $Di\n";
   	   }
   	}
   }
   #}

@Instance = ("UF1", "UF2", "UF3", "UF4", "UF5", "UF6", "UF7");
my $nobj=2;
#for(my $t=1; $t<=10; $t++)
#{
#$neighboursize=$t*10;
   foreach(@Instance)
   {
   	   my $nvar=30;
   	   for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
   	   {
		   #print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes  $neighboursize $prob $nvar $Di\n";
   	   	print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes $neighboursize $prob $limit $nvar\n";
   	   }
   }
   #}

@Instance = ("UF8", "UF9", "UF10");
$nobj=3;
#for(my $t=1; $t<=10; $t++)
#{
#  $neighboursize=$t*10;
   foreach(@Instance)
   {
   	   my $nvar=30;
   	
   	   for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
   	   {
		   #print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes $neighboursize $prob $nvar $Di\n";
   	   	print $fout "~$PathAlgorithm/moea $PathAlgorithm $_ $Sed $nobj $pops $max_nfes $neighboursize $prob $limit $nvar\n";
   	   }
   }
   #}
