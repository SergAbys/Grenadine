{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 327.0, 143.0, 375.0, 363.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 327.0, 143.0, 375.0, 363.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 14.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 20.0, 20.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 0,
		"boxanimatetime" : 200,
		"imprint" : 1,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"patching_rect" : [ 81.0, 121.0, 37.0, 21.0 ],
					"outlettype" : [ "" ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"background" : 0,
					"numinlets" : 2,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"gradient" : 0,
					"numoutlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 121.0, 260.0, 38.0, 23.0 ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"numoutlets" : 0,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l",
					"patching_rect" : [ 110.0, 215.0, 21.0, 23.0 ],
					"outlettype" : [ "" ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 40.0, 100.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"presentation" : 0,
					"id" : "obj-5",
					"blinkcolor" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"bgcolor" : [ 0.91328, 0.91328, 0.91328, 0.75 ],
					"background" : 0,
					"numinlets" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"outlinecolor" : [ 0.71328, 0.71328, 0.71328, 1.0 ],
					"numoutlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"patching_rect" : [ 200.0, 160.0, 68.0, 23.0 ],
					"outlettype" : [ "bang" ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "recall",
					"patching_rect" : [ 200.0, 200.0, 44.0, 21.0 ],
					"outlettype" : [ "" ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"background" : 0,
					"numinlets" : 2,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"gradient" : 0,
					"numoutlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Wednesday 8, 2012 / 16:35",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 45.0, 227.0, 23.0 ],
					"presentation" : 0,
					"underline" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"background" : 0,
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"patching_rect" : [ 36.0, 302.0, 52.0, 23.0 ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"background" : 0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"numoutlets" : 0,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 675.0, 308.0, 449.0, 189.0 ],
						"bglocked" : 0,
						"defrect" : [ 675.0, 308.0, 449.0, 189.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 20.0, 20.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "+ 1",
									"patching_rect" : [ 280.0, 100.0, 32.5, 23.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-14",
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 11",
									"patching_rect" : [ 320.0, 80.0, 89.0, 23.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"patching_rect" : [ 280.0, 40.0, 117.0, 23.0 ],
									"outlettype" : [ "int", "", "" ],
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"arrow" : 0,
									"id" : "obj-12",
									"fontname" : "Arial",
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
									"arrowframe" : 0,
									"numinlets" : 1,
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"fontsize" : 14.0,
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"align" : 1,
									"numoutlets" : 3,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"types" : [  ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout",
									"patching_rect" : [ 280.0, 140.0, 57.0, 23.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "freebang",
									"patching_rect" : [ 420.0, 20.0, 66.0, 23.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"numinlets" : 0,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"patching_rect" : [ 420.0, 100.0, 36.0, 23.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 16 1",
									"patching_rect" : [ 420.0, 60.0, 72.0, 23.0 ],
									"outlettype" : [ "bang", "bang", "int" ],
									"id" : "obj-17",
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ctlout 123",
									"patching_rect" : [ 420.0, 140.0, 72.0, 23.0 ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"numinlets" : 3,
									"fontsize" : 14.0,
									"numoutlets" : 0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"patching_rect" : [ 20.0, 140.0, 235.0, 23.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 3,
									"fontsize" : 14.0,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 90 250 1 @repeatmode 1",
									"patching_rect" : [ 20.0, 100.0, 235.0, 23.0 ],
									"outlettype" : [ "float", "float", "float" ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 4,
									"fontsize" : 14.0,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"outlettype" : [ "list" ],
									"id" : "obj-1",
									"numinlets" : 0,
									"numoutlets" : 1,
									"comment" : "(int)"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 2 ],
									"destination" : [ "obj-19", 2 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 2 ],
									"destination" : [ "obj-4", 2 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontname" : "Arial",
						"default_fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontsize" : 14.0,
						"fontface" : 0,
						"fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 71.0, 180.0, 40.0, 23.0 ],
					"outlettype" : [ "list", "" ],
					"presentation" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"background" : 0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 1,
					"fontface" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 14.0,
					"ignoreclick" : 0,
					"numoutlets" : 2,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 338.0, 303.0, 297.0, 320.0 ],
						"bglocked" : 0,
						"defrect" : [ 338.0, 303.0, 297.0, 320.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 20.0, 20.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "outlet",
									"patching_rect" : [ 140.0, 280.0, 25.0, 25.0 ],
									"id" : "obj-4",
									"numinlets" : 1,
									"numoutlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"patching_rect" : [ 20.0, 280.0, 25.0, 25.0 ],
									"id" : "obj-3",
									"numinlets" : 1,
									"numoutlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-2",
									"numinlets" : 0,
									"numoutlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "tralala",
									"patching_rect" : [ 20.0, 60.0, 250.0, 200.0 ],
									"yoffset" : 799,
									"outlettype" : [ "list", "", "", "bang", "" ],
									"id" : "obj-1",
									"numinlets" : 1,
									"numoutlets" : 5,
									"tralala" : 									{
										"untitled" : 										{
											"chance" : [ "chance", 100 ],
											"velocity" : [ "velocity", 0 ],
											"channel" : [ "channel", 1 ],
											"chord" : [ "chord", 0 ],
											"cell" : [ "cell", 12 ],
											"value" : [ "value", 6 ],
											"scale" : [ "scale", "C", "none" ],
											"bpm" : [ "bpm", 240 ],
											"zone" : [ "zone", 0, 96, 48, 59 ],
											"0" : [ "note", 0, 59, 74, 6, 0 ],
											"1" : [ "note", 60, 50, 80, 6, 0 ],
											"2" : [ "note", 84, 48, 80, 6, 0 ],
											"3" : [ "note", 36, 52, 78, 6, 0 ],
											"4" : [ "note", 12, 48, 86, 6, 0 ],
											"5" : [ "note", 24, 54, 78, 6, 0 ],
											"6" : [ "note", 48, 57, 79, 6, 0 ],
											"7" : [ "note", 72, 55, 86, 6, 0 ]
										}

									}

								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontname" : "Arial",
						"default_fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontsize" : 14.0,
						"fontface" : 0,
						"fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Timestamp",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 13.0, 122.0, 30.0 ],
					"presentation" : 0,
					"underline" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"background" : 0,
					"numinlets" : 1,
					"fontface" : 3,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 20.871338,
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"patching_rect" : [ 10.0, 9.0, 355.0, 64.0 ],
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"shadow" : 0,
					"mode" : 1,
					"presentation" : 0,
					"border" : 0,
					"rounded" : 8,
					"id" : "obj-23",
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"background" : 1,
					"numinlets" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"numoutlets" : 0,
					"angle" : 0.0,
					"hidden" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 90.0, 260.0, 35.0, 163.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
