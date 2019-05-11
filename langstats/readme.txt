
Programs and data to determine the bigrams frequencies for extending
mozilla libcharsetdetect to other languages (for the "Two-Char Sequence
Distribution Method")

Steps:
 - Choose langage charset pair (ie: french/cp1252)

 - Assemble a big chunk of text in the appropriate language and charset
   (fetch from ebooks, wikipedia, whatever, use iconv as needed)

 - Produce character frequency table by running charstats on the chunk, as:
   mkcharstats french/french_cp1252.txt | sort -nr +2 > \
         french/charstats_french_cp1252.txt
   or (for other versions of sort)
   mkcharstats french/french_cp1252.txt | sort -nr -k3 > \
         french/charstats_french_cp1252.txt

 - Edit the resulting file, Just get rid of a few lines that break the
   following step (the first one, the last one and the one for space (0x20)

 - Run mkpairmodel.py to produce the c++ language model. There are two
   phases, to produce a correspondance table from code point to order in
   frequency list, then a 64x64 table listing the pair frequencies for the
   64 most common characters:
   
   mkpairmodel.py french/charstats_french_cp1252.txt \
                  french/french_cp1252.txt             > LangFrenchModel.cpp

 - Integrate with the lib c++ code (3 files to change to resize the array,
   declare/define the tables: nsSBCharSetProber.h, nsSBCSGroupProber.cpp
   nsSBCSGroupProber.h
