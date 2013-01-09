# Grenadine

Grenadine is a project for generative music with agents and machine-learning ; currently just a collection of prototypes for Max/MSP.

## Thanks

- "Computing repeated factors with a factor oracle", Arnaud Lefebvre, Thierry Lecroq.
- "Factor oracle, suffix oracle", Cyril Allauzen, Maxime Crochemore, Mathieu Raffinot.
- "Incremental concept formation algorithms based on Galois (concept) lattices", Robert Godin, Rokia Missaoui, Hassan Alaoui.
- "Growing neural gas, experiments with GNG, GNG with utility and supervised GNG", Jim Holmström.
- "Apprentissage d'une chaîne de markov cachée", T. Brouard, M. Slimane, J.-P. Asselin de Beauville, G. Venturini.
  
## Installation

To install just add the "MaxMSP" folder in your filepath. For now, those externals are for macintosh only.  

## Contributing

The Cecill-C licence is LGPL-like. Contributions are welcomed, feedbacks appreciated.

## Features

### Various

```
[charlie]   : Kohonen map.
[foxtrot]   : Hidden markov model.
[romeo]     : Galois lattice.
[uniform]   : Finite state automata.
[yankee]    : Growing neural gas.
[zoulou]    : Factor oracle.
```

### Tralala

```
[quebec]    : To grab patcher notifications.
[tralala]   : Algorithmic sequencer. Updated last : January 6, 2013.
```

### Memorandum

```
Cmd : create a note.
Alt : duplicate the notes.
Ctrl {Shift} {Fn} Arrows : change values.
Caps : select the zone.

thirty-second_triplet   : 2
thirty-second           : 3
sixteenth_triplet       : 4
sixteenth               : 6
sixteenth_dotted        : 9
eighth_triplet          : 8
eighth                  : 12
eighth_dotted           : 18
quarter_triplet         : 16
quarter                 : 24
quarter_dotted          : 36
half_triplet            : 32
half                    : 48
half_dotted             : 72
whole_triplet           : 64
whole                   : 96
whole_dotted            : 144

forget
bpm
chance
velocity
channel
chord
cell
value
scale
mute
zone
note
clear
clean
transpose
rotate
scramble
sort
change
fill
kill
cycle
pattern
zoulou
romeo
juliet
dump
attributes
statistics
learn

none
ionian
dorian
phrygian
lydian
mixolydian
aeolian
locrian
natural
harmonic
melodic
whole_tone
pentatonic_major
pentatonic_minor
octatonic_half_whole
octatonic_whole_half
7th_major
7th_minor
7th_dominant
7th_major_sixth
7th_minor_sixth
7th_half_diminished
7th_diminished
7th_dominant_suspended
7th_dominant_sharp_five
7th_dominant_flat_five

none
whole
half
quarter
eighth
sixteenth
thirty-second
whole_triplet
half_triplet
quarter_triplet
eighth_triplet
sixteenth_triplet
thirty-second_triplet
whole_dotted
half_dotted
quarter_dotted
eighth_dotted
sixteenth_dotted

up
down

pitch
velocity
duration
channel

C
C#
D
D#
E
F
F#
G
G#
A
A#
B
```