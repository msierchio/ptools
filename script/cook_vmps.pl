#!/usr/bin/perl

my $vmps_vlan_file="/var/dns/sf/root/sf.vlans";
my %vlan_id_name;

open(VMPS,$vmps_vlan_file) || die "I couldn't open $vmps_vlan_file!";
while (<VMPS>) {
	my ($vlan_id,$vlan_name) = split;
	$vlan_id_name{$vlan_id} = $vlan_name;
}

my %macs;

for my $vlan_id (keys %vlan_id_name) {
	open(DHCPD,"/usr/local/etc/dhcpd-${vlan_id}.conf") 
		|| die "I couldn't open /usr/local/etc/dhcpd-${vlan_id}.conf!";
	while(<DHCPD>) {
		next if ( ! /^host.*fixed-address/ );
		my ($hostname,$mac) = (split)[1,5];
		$mac =~ tr/A-Z/a-z/;
		$mac =~ s/[:;]//g;
		$mac =~ s/(....)(....)(....)/\1.\2.\3/g;
		die "\n########\nduplicate mac $mac in /usr/local/etc/dhcpd-${vlan_id}.conf\n########\n\n" if exists $macs{$mac};
		$macs{$mac} = 1;
		print "address $mac vlan-name $vlan_id_name{$vlan_id} ! $hostname \n";
	}
}
