#Con este script se genera la lista de ejecuciones....
#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
my $file = "ExecutionFileDiversity2";
my $fout;
open($fout, '>' ,$file);
my $Path = `cd ..; pwd`; #   "/home/joel.chacon/Chacon/DE_diversity/CEC2018/";
chomp $Path;

my $Sed=0;
my $NEjecutions=35;

#my @Algorithms= ("MOEAD_main", "MOEAD_main");
#my @Algorithms= ("NSGAII_main", "NSGAII_main");
my @Algorithms= ("NSGAII_main");
#my @Algorithms= ("MOEAD_main" ,"NSGAII_main", "GDE3_main");
#my @Algorithms= ("FastSMSEMOA_main", "MOEAD_main", "NSGAII_main", "GDE3_main");
#my @Algorithms= ("NSGAII_main", "", "" ,"GDE3_main", "NSGAII_main", "GDE3_main", "NSGAII_main", "GDE3_main" );
#my @Algorithms= ("SMSEMOA_main"); #, "MOEAD_main");
#my @DirAlgorithms= ("Run_Gen250mil_Pob100_Dynamic_3Objs/SMSEMOA");#, "Run_Gen250mil_Pob100_Dynamic_3Objs/MOEAD");
#my @DirAlgorithms= ("Run_Gen250mil_Pob_100_DE_Dynamic_Hybrid/SMSEMOA", "Run_Gen250mil_Pob_100_DE_Dynamic_Hybrid/MOEAD", "Run_Gen250mil_Pob_100_DE_Dynamic_Hybrid/NSGAII", "Run_Gen250mil_Pob_100_DE_Dynamic_Hybrid/GDE3");
my @DirAlgorithms= ("NSGAII");
#my @DirAlgorithms= ("DNR-DE-Gen250th-NObjs/UMOEAD-Obj-2", "DNR-DE-Gen250th-NObjs/UMOEAD-Obj-3", "DNR-DE-Gen250th-NObjs/UMOEAD-Obj-10" );
#my @DirAlgorithms= ("DNR-DE-Gen250th-NObjs/UMOEAD-Obj-10", "DNR-DE-Gen250th-NObjs/NSGAII-Obj-10", "DNR-DE-Gen250th-NObjs/GDE3-Obj-10");#, "DNR-DE-Gen250th-NObjs/NSGAII-Obj-3", "DNR-DE-Gen250th-NObjs/GDE3-Obj-2", "DNR-DE-Gen250th-NObjs/GDE3-Obj-3" );
#
#my @NVariables = ("105", "105", "105", "105", "105", "105", "105", "105", "105","14", "19", "19", "19", "19", "19", "29");
#my @NVariables = ("11", "11","12", "12","19", "19");
#my @NVariables = ("24", "24", "24", "24", "24", "24", "24", "24", "24","7", "12", "12", "12", "12", "12", "22");
my @NVariables = ("24", "24", "24", "24", "24", "24", "24", "24", "24","6", "11", "11", "11", "11", "11", "21");
#my @NVariables = ("30", "30", "30", "10", "10");
#my @Instances= ("DTLZ3"); ##La ZDT5 es para cadenas binarias.....
my @Instances = ("WFG1","WFG2", "WFG3", "WFG4", "WFG5", "WFG6", "WFG7", "WFG8", "WFG9","DTLZ1","DTLZ2", "DTLZ3", "DTLZ4", "DTLZ5", "DTLZ6", "DTLZ7"); ##La ZDT5 es para cadenas binarias.....
#my @Instances = ("WFG2");
##my @Instances= ("ZDT1","ZDT2", "ZDT3", "ZDT4", "ZDT6"); ##La ZDT5 es para cadenas binarias.....
#
my @NObjectives = ("2");
#
#
@Algorithms= ("NSGAII_main");
@DirAlgorithms= ("NSGAII");

for(my $i = 0; $i < scalar(@Algorithms); $i++ )
{
        my $contv = 0;
	foreach my $Instance(@Instances) ##Para cada instancia WFG...
	{
		for($Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
		{
			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real ".$NVariables[$contv]." ".$NObjectives[$i]." $Sed $Path/".$DirAlgorithms[$i]." \n";
		}
	   $contv++;
	}
}
 @Instances= ("UF1", "UF2", "UF3", "UF4", "UF5", "UF6", "UF7"); 
for(my $i = 0; $i < scalar(@Algorithms); $i++ )
{
	foreach my $Instance(@Instances) 
	{
		for($Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
		{

			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real 30 2 $Sed $Path/".$DirAlgorithms[$i]." \n";
		}
	}

}

@Instances = ("WFG1","WFG2", "WFG3", "WFG4", "WFG5", "WFG6", "WFG7", "WFG8", "WFG9","DTLZ1","DTLZ2", "DTLZ3", "DTLZ4", "DTLZ5", "DTLZ6", "DTLZ7"); ##La ZDT5 es para cadenas binarias.....
@NObjectives = ("3");
 @NVariables = ("24", "24", "24", "24", "24", "24", "24", "24", "24","7", "12", "12", "12", "12", "12", "22");
for(my $i = 0; $i < scalar(@Algorithms); $i++ )
{
        my $contv = 0;
	foreach my $Instance(@Instances) ##Para cada instancia WFG...
	{
		for($Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
		{
			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real ".$NVariables[$contv]." ".$NObjectives[$i]." $Sed $Path/".$DirAlgorithms[$i]." \n";
		}
	   $contv++;
	}
}


@Instances= ("UF8", "UF9", "UF10"); 
for(my $i = 0; $i < scalar(@Algorithms); $i++ )
{
	foreach my $Instance(@Instances) 
	{
		for($Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
		{
			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real 30 3 $Sed $Path/".$DirAlgorithms[$i]." \n";
		}
	}

}
#
###sms-emoa
#
#@Algorithms= ("FastSMSEMOA_main");
#@DirAlgorithms= ("DE-Gen250th-NObjs/SMS-EMOA");
#@NObjectives = ("2");
#
#@NVariables = ("24", "24", "24", "24", "24", "24", "24", "24", "24","6", "11", "11", "11", "11", "11", "21");
#
#@Instances = ("WFG1","WFG2", "WFG3", "WFG4", "WFG5", "WFG6", "WFG7", "WFG8", "WFG9","DTLZ1","DTLZ2", "DTLZ3", "DTLZ4", "DTLZ5", "DTLZ6", "DTLZ7"); ##La ZDT5 es para cadenas binarias.....
#for(my $i = 0; $i < scalar(@Algorithms); $i++ )
#{
#        my $contv = 0;
#	foreach my $Instance(@Instances) ##Para cada instancia WFG...
#	{
#		for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
#		{
#			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real ".$NVariables[$contv]." ".$NObjectives[$i]." $Sed $Path/".$DirAlgorithms[$i]." \n";
#		}
#	   $contv++;
#	}
#}
#
#
#
# @Instances= ("UF1", "UF2", "UF3", "UF4", "UF5", "UF6", "UF7"); 
#for(my $i = 0; $i < scalar(@Algorithms); $i++ )
#{
#	foreach my $Instance(@Instances) 
#	{
#		for(my $Sed = 1; $Sed <=35; $Sed++) ##Realizar 35 ejecuciones con distintas semillas de cada instancia..
#		{
#
#			print $fout "$Sed~$Path/bin/main/".$Algorithms[$i]." $Instance Real 30 2 $Sed $Path/".$DirAlgorithms[$i]." \n";
#		}
#	}
#
#}
#
