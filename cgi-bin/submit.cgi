#!/usr/bin/env perl
use strict;
use warnings;
use CGI;

my $query = CGI->new;
my $suki = $query->param('food') || 'No parameter';
print "Content-Type: text/html\n\n";
print "<html><head>\n";
print "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n";
print "</head><body>\n";
print "$suki is delicious\n";
print "</body></html>";
