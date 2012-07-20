When our users load a module from the list at http://rc.fas.harvard.edu/module_list we increment the count in a MySQL database.

Afterwards, we can query the database with `checkmodule` as described at http://blog.jcuff.net/2012/07/part-two-scientific-software-as-service.html

    [jcuff@odyssey ~]$ checkmodule -u jcuff | head

    Name     Module              Count     Time
    jcuff    hpc/rc              19        Thu Jul 19 13:45:53 2012
    jcuff    viz/gnuplot-4.5     135       Thu Jul 19 13:45:45 2012
    jcuff    hpc/blat-34         150       Thu Jul 19 13:45:45 2012
    jcuff    hpc/blastall        146       Thu Jul 19 13:45:45 2012
    jcuff    hpc/abyss-1.2.1     242       Thu Jul 19 13:45:45 2012
    jcuff    hpc/openmpi-intel   421       Thu Jul 19 13:45:45 2012
    jcuff    bio/samtools-0.1.7  127       Thu Jul 19 13:45:45 2012
    jcuff    bio/bowtie-0.11.3   150       Thu Jul 19 13:45:45 2012

    [SNIP]

See also http://blog.jcuff.net/2012/07/scientific-software-as-service-sprawl.html and https://github.com/fasrc/api/blob/master/modules
