#!/usr/bin/perl
# convert to MNL transaction format
# 1) change ASIN to integer id

use strict;
use warnings;

my %itemIdMap = ();
my $maxId = 0;
my $transactionFile = "./trans.txt";
my $idMapFile = "./prodIdMap.txt";

open TRANS_FILE, ">", $transactionFile or die $!;
open MAP_FILE, ">", $idMapFile or die $!;

my $transCnt = 0;
while(<>) {
	chomp;
	my ($itemCnt,$setCnt) = split /\s+/;
	# include original set
	$setCnt++;
	print  TRANS_FILE join(" ", ($itemCnt, $setCnt)) . "\n";
	my $line = <>;
	chomp $line;
	my @items = split /\s+/, $line;
	my @itemIds = ();
	foreach my $item (@items) {
		if (not exists $itemIdMap{$item}) {
			$itemIdMap{$item} = $maxId;
			print MAP_FILE join(" ",($item,$maxId)) . "\n";
			$maxId++;
		}
		push @itemIds, $itemIdMap{$item};
	}
	print TRANS_FILE join(" ",@itemIds) . "\n";
	# price line, skipped here
	$line = <>;
	for (my $i = 0; $i < $setCnt; $i++) {
		$line = <>;
		chomp $line;
		# get the quantity assignments
		my @quants = split /\s+/,$line;
		print TRANS_FILE join(" ",@quants) . "\n";
	}
	$transCnt++;
#	if ($transCnt == 100) {
#		last;
#	}
}

close TRANS_FILE;
close MAP_FILE;
