{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 467.0, 101.0, 572.0, 438.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 467.0, 101.0, 572.0, 438.0 ],
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
					"maxclass" : "newobj",
					"text" : "p init",
					"patching_rect" : [ 18.0, 376.0, 40.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"hidden" : 0,
					"id" : "obj-5",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 25.0, 69.0, 195.0, 143.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 195.0, 143.0 ],
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
									"text" : "loadbang",
									"patching_rect" : [ 20.0, 20.0, 68.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : ";\rtoTralala recall",
									"linecount" : 2,
									"patching_rect" : [ 20.0, 60.0, 102.0, 37.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-8"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : "",
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "zoulou",
					"patching_rect" : [ 142.0, 161.0, 52.0, 21.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 2,
					"fontface" : 0,
					"background" : 0,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"fontsize" : 14.0,
					"numoutlets" : 1,
					"gradient" : 0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"outlettype" : [ "" ],
					"hidden" : 0,
					"id" : "obj-18"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l",
					"patching_rect" : [ 51.0, 161.0, 21.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 1,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numoutlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"outlettype" : [ "" ],
					"hidden" : 0,
					"id" : "obj-15"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"patching_rect" : [ 80.0, 376.0, 52.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"hidden" : 0,
					"id" : "obj-16",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 986.0, 177.0, 165.0, 209.0 ],
						"bglocked" : 0,
						"defrect" : [ 986.0, 177.0, 165.0, 209.0 ],
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
									"patching_rect" : [ 20.0, 100.0, 32.5, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"id" : "obj-14"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 74",
									"patching_rect" : [ 60.0, 80.0, 90.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"hidden" : 1,
									"id" : "obj-10"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"patching_rect" : [ 20.0, 40.0, 117.0, 23.0 ],
									"fontname" : "Arial",
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"numinlets" : 1,
									"arrow" : 0,
									"types" : [  ],
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"align" : 1,
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
									"outlettype" : [ "int", "", "" ],
									"arrowframe" : 0,
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"id" : "obj-12"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout 1",
									"patching_rect" : [ 20.0, 140.0, 69.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 0,
									"id" : "obj-13"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "freebang",
									"patching_rect" : [ 160.0, 20.0, 66.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 0,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"hidden" : 1,
									"id" : "obj-21"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"patching_rect" : [ 160.0, 100.0, 36.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"hidden" : 1,
									"id" : "obj-18"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 16 1",
									"patching_rect" : [ 160.0, 60.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "bang", "int" ],
									"hidden" : 1,
									"id" : "obj-17"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ctlout 123",
									"patching_rect" : [ 160.0, 140.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 3,
									"fontsize" : 14.0,
									"numoutlets" : 0,
									"hidden" : 1,
									"id" : "obj-19"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-12", 0 ],
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
									"source" : [ "obj-17", 2 ],
									"destination" : [ "obj-19", 2 ],
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
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-18", 0 ],
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
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : "",
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rtoAll reset",
					"linecount" : 2,
					"patching_rect" : [ 280.0, 360.0, 73.0, 37.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 2,
					"fontface" : 0,
					"background" : 0,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"fontsize" : 14.0,
					"numoutlets" : 1,
					"gradient" : 0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"outlettype" : [ "" ],
					"hidden" : 0,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop",
					"patching_rect" : [ 40.0, 100.0, 37.0, 21.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 2,
					"fontface" : 0,
					"background" : 0,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"fontsize" : 14.0,
					"numoutlets" : 1,
					"gradient" : 0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"outlettype" : [ "" ],
					"hidden" : 0,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 80.0, 160.0, 54.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 1,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numoutlets" : 4,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"outlettype" : [ "list", "", "", "bang" ],
					"hidden" : 0,
					"id" : "obj-1",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 642.0, 210.0, 330.0, 279.0 ],
						"bglocked" : 0,
						"defrect" : [ 642.0, 210.0, 330.0, 279.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 1,
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
						"title" : "Tralala",
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 180.0, 340.0, 59.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numoutlets" : 0,
									"id" : "obj-12",
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 25.0, 69.0, 163.0, 254.0 ],
										"bglocked" : 0,
										"defrect" : [ 25.0, 69.0, 163.0, 254.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "s fromTralala",
													"patching_rect" : [ 20.0, 200.0, 92.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 1,
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numoutlets" : 0,
													"id" : "obj-3"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend",
													"patching_rect" : [ 20.0, 140.0, 62.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 1,
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-5"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"patching_rect" : [ 80.0, 80.0, 49.0, 21.0 ],
													"fontname" : "Arial",
													"numinlets" : 2,
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-4"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 80.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-2",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-1",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontname" : "Arial",
										"default_fontface" : 0,
										"globalpatchername" : "",
										"default_fontname" : "Arial",
										"fontface" : 0,
										"fontsize" : 14.0,
										"default_fontsize" : 14.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"annotation" : "",
									"patching_rect" : [ 140.0, 420.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-10",
									"comment" : "Will End"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"annotation" : "",
									"patching_rect" : [ 100.0, 420.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-7",
									"comment" : "End"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"annotation" : "",
									"patching_rect" : [ 60.0, 420.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-6",
									"comment" : "Dumped Notes"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"annotation" : "",
									"patching_rect" : [ 20.0, 420.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-5",
									"comment" : "Played Notes"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "tralala",
									"patching_rect" : [ 20.0, 220.0, 179.0, 86.0 ],
									"presentation" : 1,
									"yoffset" : 879,
									"numinlets" : 1,
									"numoutlets" : 5,
									"presentation_rect" : [ 40.0, 40.0, 250.0, 200.0 ],
									"outlettype" : [ "list", "", "", "bang", "" ],
									"id" : "obj-4",
									"tralala" : 									{
										"untitled" : 										{
											"zone" : [ "zone", 0, 96, 40, 51 ],
											"0" : [ "note", 84, 48, 80, 3, 0 ],
											"1" : [ "note", 24, 48, 75, 3, 0 ],
											"2" : [ "note", 60, 48, 80, 3, 0 ],
											"3" : [ "note", 30, 45, 80, 3, 0 ],
											"4" : [ "note", 6, 47, 75, 3, 0 ],
											"5" : [ "note", 12, 45, 80, 3, 0 ],
											"6" : [ "note", 36, 47, 95, 3, 0 ],
											"7" : [ "note", 78, 47, 80, 3, 0 ],
											"8" : [ "note", 48, 48, 80, 3, 0 ],
											"9" : [ "note", 18, 47, 95, 3, 0 ],
											"10" : [ "note", 54, 47, 95, 3, 0 ],
											"11" : [ "note", 72, 45, 80, 3, 0 ],
											"12" : [ "note", 0, 48, 70, 3, 0 ],
											"13" : [ "note", 66, 47, 75, 3, 0 ],
											"14" : [ "note", 42, 47, 75, 3, 0 ],
											"15" : [ "note", 90, 45, 75, 3, 0 ],
											"bpm" : [ "bpm", 60 ],
											"chance" : [ "chance", 100 ],
											"velocity" : [ "velocity", 0 ],
											"channel" : [ "channel", 1 ],
											"chord" : [ "chord", 0 ],
											"cell" : [ "cell", 6 ],
											"value" : [ "value", 6 ],
											"scale" : [ "scale", "E", "natural" ],
											"mute" : [ "mute", 0 ]
										}

									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 20.0, 340.0, 29.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"id" : "obj-26",
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 324.0, 259.0, 337.0, 201.0 ],
										"bglocked" : 0,
										"defrect" : [ 324.0, 259.0, 337.0, 201.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "outlet",
													"annotation" : "",
													"patching_rect" : [ 20.0, 160.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-1",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "noteout",
													"patching_rect" : [ 60.0, 160.0, 255.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 3,
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numoutlets" : 0,
													"id" : "obj-7"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "makenote 60 90 250 1 @repeatmode 1",
													"patching_rect" : [ 60.0, 120.0, 255.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 4,
													"fontsize" : 14.0,
													"numoutlets" : 3,
													"outlettype" : [ "float", "float", "float" ],
													"id" : "obj-19"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "list" ],
													"id" : "obj-21",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 2 ],
													"destination" : [ "obj-7", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 1 ],
													"destination" : [ "obj-7", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontname" : "Arial",
										"default_fontface" : 0,
										"globalpatchername" : "",
										"default_fontname" : "Arial",
										"fontface" : 0,
										"fontsize" : 14.0,
										"default_fontsize" : 14.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 20.0, 140.0, 219.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numoutlets" : 3,
									"outlettype" : [ "", "", "" ],
									"id" : "obj-23",
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 384.0, 118.0, 408.0, 327.0 ],
										"bglocked" : 0,
										"defrect" : [ 384.0, 118.0, 408.0, 327.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p",
													"patching_rect" : [ 20.0, 240.0, 40.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 1,
													"fontsize" : 14.0,
													"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-5",
													"patcher" : 													{
														"fileversion" : 1,
														"appversion" : 														{
															"major" : 5,
															"minor" : 1,
															"revision" : 9
														}
,
														"rect" : [ 635.0, 112.0, 326.0, 223.0 ],
														"bglocked" : 0,
														"defrect" : [ 635.0, 112.0, 326.0, 223.0 ],
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
														"boxes" : [ 															{
																"box" : 																{
																	"maxclass" : "message",
																	"text" : "resave",
																	"patching_rect" : [ 240.0, 120.0, 53.0, 21.0 ],
																	"fontname" : "Arial",
																	"numinlets" : 2,
																	"fontsize" : 14.0,
																	"numoutlets" : 1,
																	"outlettype" : [ "" ],
																	"id" : "obj-6"
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "sel 115",
																	"patching_rect" : [ 240.0, 80.0, 55.0, 23.0 ],
																	"fontname" : "Arial",
																	"numinlets" : 2,
																	"fontsize" : 14.0,
																	"numoutlets" : 2,
																	"outlettype" : [ "bang", "" ],
																	"id" : "obj-7"
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "key",
																	"patching_rect" : [ 200.0, 20.0, 58.5, 23.0 ],
																	"fontname" : "Arial",
																	"numinlets" : 0,
																	"fontsize" : 14.0,
																	"numoutlets" : 4,
																	"outlettype" : [ "int", "int", "int", "int" ],
																	"id" : "obj-14"
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "message",
																	"text" : "stop, forget, recall",
																	"patching_rect" : [ 80.0, 120.0, 122.0, 21.0 ],
																	"fontname" : "Arial",
																	"numinlets" : 2,
																	"fontsize" : 14.0,
																	"numoutlets" : 1,
																	"outlettype" : [ "" ],
																	"id" : "obj-5"
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "sel reset",
																	"patching_rect" : [ 80.0, 80.0, 63.0, 23.0 ],
																	"fontname" : "Arial",
																	"numinlets" : 1,
																	"fontsize" : 14.0,
																	"numoutlets" : 2,
																	"outlettype" : [ "bang", "" ],
																	"id" : "obj-3"
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"patching_rect" : [ 20.0, 180.0, 25.0, 25.0 ],
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"id" : "obj-2",
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"outlettype" : [ "" ],
																	"id" : "obj-1",
																	"comment" : ""
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-5", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-3", 0 ],
																	"destination" : [ "obj-5", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-1", 0 ],
																	"destination" : [ "obj-3", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-1", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-14", 3 ],
																	"destination" : [ "obj-7", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-7", 0 ],
																	"destination" : [ "obj-6", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-6", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [ 249.5, 158.0, 29.5, 158.0 ]
																}

															}
 ]
													}
,
													"saved_object_attributes" : 													{
														"fontname" : "Arial",
														"default_fontface" : 0,
														"globalpatchername" : "",
														"default_fontname" : "Arial",
														"fontface" : 0,
														"fontsize" : 14.0,
														"default_fontsize" : 14.0
													}

												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "r toAll",
													"patching_rect" : [ 60.0, 120.0, 46.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 0,
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-2"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 360.0, 280.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-1",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "r toTralala",
													"patching_rect" : [ 80.0, 160.0, 73.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 0,
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-12"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 20.0, 280.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-6",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-9",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "r",
													"patching_rect" : [ 220.0, 160.0, 33.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 1,
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-4"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"patching_rect" : [ 220.0, 120.0, 49.0, 21.0 ],
													"fontname" : "Arial",
													"numinlets" : 2,
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-3"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "title $1",
													"patching_rect" : [ 280.0, 120.0, 52.0, 21.0 ],
													"fontname" : "Arial",
													"numinlets" : 2,
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-17"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "route title",
													"patching_rect" : [ 220.0, 60.0, 68.0, 23.0 ],
													"fontname" : "Arial",
													"numinlets" : 1,
													"fontsize" : 14.0,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"id" : "obj-10"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 220.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-20",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 280.0, 280.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-22",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [ 229.5, 97.0, 289.5, 97.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-17", 0 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [ 229.5, 97.0, 369.5, 97.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 229.5, 204.0, 29.5, 204.0 ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontname" : "Arial",
										"default_fontface" : 0,
										"globalpatchername" : "",
										"default_fontname" : "Arial",
										"fontface" : 0,
										"fontsize" : 14.0,
										"default_fontsize" : 14.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "thispatcher",
									"patching_rect" : [ 120.0, 180.0, 79.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"id" : "obj-9",
									"save" : [ "#N", "thispatcher", ";", "#Q", "end", ";" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "quebec",
									"patching_rect" : [ 180.0, 100.0, 59.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 1,
									"fontsize" : 14.0,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"id" : "obj-11"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 1",
									"patching_rect" : [ 180.0, 60.0, 40.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 2,
									"fontsize" : 14.0,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"id" : "obj-8"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "active",
									"patching_rect" : [ 180.0, 20.0, 47.0, 23.0 ],
									"fontname" : "Arial",
									"numinlets" : 0,
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"hint" : "",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-1",
									"comment" : "(anything) messages"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-4", 3 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 2 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-23", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 1 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 2 ],
									"destination" : [ "obj-12", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 4 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : "",
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 45.0, 227.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 1,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"underline" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"ignoreclick" : 1,
					"hidden" : 0,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 13.0, 122.0, 30.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"numinlets" : 1,
					"fontface" : 3,
					"background" : 0,
					"fontsize" : 20.871338,
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"underline" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"ignoreclick" : 1,
					"hidden" : 0,
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"patching_rect" : [ 10.0, 9.0, 355.0, 64.0 ],
					"mode" : 1,
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"presentation" : 0,
					"border" : 0,
					"rounded" : 8,
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"numinlets" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"background" : 1,
					"numoutlets" : 0,
					"shadow" : 0,
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"angle" : 0.0,
					"hidden" : 0,
					"id" : "obj-23"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 3 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
