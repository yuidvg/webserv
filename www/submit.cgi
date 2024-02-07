#!/usr/bin/env perl
use CGI;

$query = CGI->new;
$suki = $query->param('food');
print "Content-Type: text/html\n\n";
print "<html><head>\n";
print "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=EUC-JP\">\n";
print "</head><body>\n";
print "$sukiっておいしいですよね!\n";
print"</body></html>";
