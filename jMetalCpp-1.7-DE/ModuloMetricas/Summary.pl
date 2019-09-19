

my @LABELS;
push(@LABELS, 'NSGAII_SBX_9_Polynomial_1_Size_100_Gen_25000_');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_25000_D1');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_25000_D2.5');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_25000_D5');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_25000_D7.5');


foreach my $l (@LABELS)
{
	for(my $Instance = 1; $Instance <=9; $Instance++)
	{
		print `./Ejecutable --i ../Prueba/WFG$Instance/ --l $l --o ../Summary/WFG$Instance --pf OptimalFronts/InstancesWFG_ParamDistance20_Position_4/WFG$Instance.txt --HYPERVOLUME 3,5 --IGD`;
	print "Instance WFG$Instance $l Ready!!...\n";
	}
}

@LABELS = ();

push(@LABELS, 'NSGAII_SBX_9_Polynomial_1_Size_100_Gen_250000');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_250000_D1');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_250000_D2.5');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_250000_D5');
push(@LABELS, 'NSGAII_Diversity_SBX_9_Polynomial_1_Size_100_Gen_250000_D7.5');

foreach my $l (@LABELS)
{
	for(my $Instance = 1; $Instance <=9; $Instance++)
	{
		print `./Ejecutable --i ../Prueba/WFG$Instance/ --l $l --o ../Summary/WFG$Instance --pf OptimalFronts/InstancesWFG_ParamDistance20_Position_4/WFG$Instance.txt --HYPERVOLUME 3,5 --IGD`;
	print "Instance WFG$Instance $l Ready!!...\n";
	}
}
