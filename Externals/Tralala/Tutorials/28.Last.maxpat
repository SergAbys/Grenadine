{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 178.0, 122.0, 375.0, 363.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 178.0, 122.0, 375.0, 363.0 ],
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
					"text" : ";\rtoto statistics",
					"linecount" : 2,
					"patching_rect" : [ 180.0, 82.0, 91.0, 37.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"gradient" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-7",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"patching_rect" : [ 134.0, 291.0, 52.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 0,
					"hidden" : 0,
					"id" : "obj-18",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 675.0, 308.0, 221.0, 182.0 ],
						"bglocked" : 0,
						"defrect" : [ 675.0, 308.0, 221.0, 182.0 ],
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
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-14",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 74",
									"patching_rect" : [ 60.0, 80.0, 90.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"hidden" : 1,
									"id" : "obj-10",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"patching_rect" : [ 20.0, 40.0, 117.0, 23.0 ],
									"arrow" : 0,
									"fontname" : "Arial",
									"align" : 1,
									"arrowframe" : 0,
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"types" : [  ],
									"fontsize" : 14.0,
									"numinlets" : 1,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"numoutlets" : 3,
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"id" : "obj-12",
									"outlettype" : [ "int", "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout",
									"patching_rect" : [ 20.0, 140.0, 57.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
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
									"fontsize" : 14.0,
									"numinlets" : 0,
									"numoutlets" : 1,
									"hidden" : 1,
									"id" : "obj-21",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"patching_rect" : [ 160.0, 100.0, 36.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"hidden" : 1,
									"id" : "obj-18",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 16 1",
									"patching_rect" : [ 160.0, 60.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 3,
									"hidden" : 1,
									"id" : "obj-17",
									"outlettype" : [ "bang", "bang", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ctlout 123",
									"patching_rect" : [ 160.0, 140.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 3,
									"numoutlets" : 0,
									"hidden" : 1,
									"id" : "obj-19"
								}

							}
 ],
						"lines" : [ 							{
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
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-12", 0 ],
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
					"maxclass" : "newobj",
					"text" : "loadbang",
					"patching_rect" : [ 35.0, 152.0, 68.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 1,
					"id" : "obj-12",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "recall",
					"patching_rect" : [ 35.0, 192.0, 44.0, 21.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"gradient" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 1,
					"id" : "obj-9",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rtoto reset",
					"linecount" : 2,
					"patching_rect" : [ 95.0, 82.0, 69.0, 37.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"gradient" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-16",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 149.0, 192.0, 38.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 0,
					"hidden" : 0,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r fromTralala",
					"patching_rect" : [ 149.0, 152.0, 89.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
					"numinlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-5",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p toto",
					"patching_rect" : [ 51.0, 250.0, 46.5, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 4,
					"hidden" : 0,
					"id" : "obj-2",
					"outlettype" : [ "list", "", "", "bang" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 590.0, 293.0, 427.0, 275.0 ],
						"bglocked" : 0,
						"defrect" : [ 590.0, 293.0, 427.0, 275.0 ],
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
						"title" : "toto",
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 180.0, 420.0, 59.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 2,
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
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numinlets" : 1,
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
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-5",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"patching_rect" : [ 80.0, 80.0, 49.0, 21.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-4",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 80.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-2",
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-1",
													"outlettype" : [ "" ],
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
									"patching_rect" : [ 140.0, 500.0, 25.0, 25.0 ],
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
									"patching_rect" : [ 100.0, 500.0, 25.0, 25.0 ],
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
									"patching_rect" : [ 60.0, 500.0, 25.0, 25.0 ],
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
									"patching_rect" : [ 20.0, 500.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-5",
									"comment" : "Played Notes"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "tralala",
									"patching_rect" : [ 20.0, 300.0, 179.0, 86.0 ],
									"presentation" : 1,
									"yoffset" : 654,
									"numinlets" : 1,
									"presentation_rect" : [ 40.0, 40.0, 350.0, 200.0 ],
									"utextcolor" : [ 0.51, 0.44, 0.49, 0.0 ],
									"numoutlets" : 5,
									"id" : "obj-4",
									"outlettype" : [ "list", "", "", "bang", "" ],
									"tralala" : 									{
										"untitled" : 										{
											"bpm" : [ "bpm", 120 ],
											"chance" : [ "chance", 100 ],
											"velocity" : [ "velocity", 0 ],
											"channel" : [ "channel", 1 ],
											"cell" : [ "cell", 12 ],
											"value" : [ "value", 12 ],
											"scale" : [ "scale", "C", "none" ],
											"mute" : [ "mute", 0 ],
											"zone" : [ "zone", 0, 96, 60, 71 ],
											"4" : [ "note", 24, 60, 80, 12, 0 ],
											"0" : [ "note", 60, 60, 80, 12, 0 ],
											"3" : [ "note", 84, 60, 80, 12, 0 ],
											"1" : [ "note", 72, 62, 80, 12, 0 ],
											"8" : [ "note", 0, 63, 80, 12, 0 ],
											"9" : [ "note", 48, 62, 80, 12, 0 ],
											"10" : [ "note", 12, 62, 80, 12, 0 ],
											"11" : [ "note", 36, 63, 80, 12, 0 ],
											"2" : [ "note", 12, 63, 80, 12, 0 ],
											"6" : [ "note", 48, 63, 80, 12, 0 ],
											"chord" : [ "chord", 1 ],
											"5" : [ "note", 0, 65, 80, 12, 0 ],
											"7" : [ "note", 36, 65, 80, 12, 0 ]
										}

									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 20.0, 420.0, 29.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-26",
									"outlettype" : [ "list" ],
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
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numinlets" : 3,
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
													"fontsize" : 14.0,
													"numinlets" : 4,
													"numoutlets" : 3,
													"id" : "obj-19",
													"outlettype" : [ "float", "float", "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-21",
													"outlettype" : [ "list" ],
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
									"patching_rect" : [ 20.0, 180.0, 219.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-23",
									"outlettype" : [ "", "", "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 254.0, 107.0, 485.0, 322.0 ],
										"bglocked" : 0,
										"defrect" : [ 254.0, 107.0, 485.0, 322.0 ],
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
													"maxclass" : "inlet",
													"patching_rect" : [ 120.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-7",
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p",
													"patching_rect" : [ 20.0, 240.0, 49.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-5",
													"outlettype" : [ "" ],
													"patcher" : 													{
														"fileversion" : 1,
														"appversion" : 														{
															"major" : 5,
															"minor" : 1,
															"revision" : 9
														}
,
														"rect" : [ 206.0, 126.0, 298.0, 252.0 ],
														"bglocked" : 0,
														"defrect" : [ 206.0, 126.0, 298.0, 252.0 ],
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
																	"maxclass" : "newobj",
																	"text" : "p",
																	"patching_rect" : [ 200.0, 120.0, 43.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
																	"numinlets" : 1,
																	"numoutlets" : 1,
																	"id" : "obj-15",
																	"outlettype" : [ "" ],
																	"patcher" : 																	{
																		"fileversion" : 1,
																		"appversion" : 																		{
																			"major" : 5,
																			"minor" : 1,
																			"revision" : 9
																		}
,
																		"rect" : [ 671.0, 102.0, 416.0, 330.0 ],
																		"bglocked" : 0,
																		"defrect" : [ 671.0, 102.0, 416.0, 330.0 ],
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
																		"boxes" : [ 																			{
																				"box" : 																				{
																					"maxclass" : "newobj",
																					"text" : "p",
																					"patching_rect" : [ 20.0, 200.0, 67.0, 23.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
																					"numinlets" : 2,
																					"numoutlets" : 1,
																					"id" : "obj-16",
																					"outlettype" : [ "" ],
																					"patcher" : 																					{
																						"fileversion" : 1,
																						"appversion" : 																						{
																							"major" : 5,
																							"minor" : 1,
																							"revision" : 9
																						}
,
																						"rect" : [ 356.0, 159.0, 559.0, 377.0 ],
																						"bglocked" : 0,
																						"defrect" : [ 356.0, 159.0, 559.0, 377.0 ],
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
																						"boxes" : [ 																							{
																								"box" : 																								{
																									"maxclass" : "inlet",
																									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
																									"numinlets" : 0,
																									"numoutlets" : 1,
																									"id" : "obj-2",
																									"outlettype" : [ "" ],
																									"comment" : ""
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "zl reg",
																									"patching_rect" : [ 20.0, 220.0, 59.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 2,
																									"id" : "obj-4",
																									"outlettype" : [ "", "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "sel resave",
																									"patching_rect" : [ 80.0, 60.0, 74.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 1,
																									"numoutlets" : 2,
																									"id" : "obj-1",
																									"outlettype" : [ "bang", "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "message",
																									"text" : "textcolor 0.51 0.44 0.49 1.",
																									"patching_rect" : [ 200.0, 220.0, 172.0, 21.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-11",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "message",
																									"text" : "utextcolor 0.83 0.74 0.84 1., textcolor 0.83 0.74 0.84 1.",
																									"patching_rect" : [ 180.0, 160.0, 350.0, 21.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-9",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "delay 350",
																									"patching_rect" : [ 80.0, 140.0, 71.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-5",
																									"outlettype" : [ "bang" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "t b b",
																									"patching_rect" : [ 80.0, 100.0, 80.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 1,
																									"numoutlets" : 2,
																									"id" : "obj-3",
																									"outlettype" : [ "bang", "bang" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "inlet",
																									"patching_rect" : [ 80.0, 20.0, 25.0, 25.0 ],
																									"numinlets" : 0,
																									"numoutlets" : 1,
																									"id" : "obj-13",
																									"outlettype" : [ "" ],
																									"comment" : ""
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "outlet",
																									"patching_rect" : [ 20.0, 320.0, 25.0, 25.0 ],
																									"numinlets" : 1,
																									"numoutlets" : 0,
																									"id" : "obj-15",
																									"comment" : ""
																								}

																							}
 ],
																						"lines" : [ 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-13", 0 ],
																									"destination" : [ "obj-1", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-1", 0 ],
																									"destination" : [ "obj-3", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-11", 0 ],
																									"destination" : [ "obj-15", 0 ],
																									"hidden" : 0,
																									"midpoints" : [ 209.5, 281.0, 29.5, 281.0 ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-3", 1 ],
																									"destination" : [ "obj-9", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-9", 0 ],
																									"destination" : [ "obj-15", 0 ],
																									"hidden" : 0,
																									"midpoints" : [ 189.5, 281.0, 29.5, 281.0 ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-4", 0 ],
																									"destination" : [ "obj-15", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-2", 0 ],
																									"destination" : [ "obj-4", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-5", 0 ],
																									"destination" : [ "obj-4", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-3", 0 ],
																									"destination" : [ "obj-5", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-5", 0 ],
																									"destination" : [ "obj-11", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
 ]
																					}
,
																					"saved_object_attributes" : 																					{
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
, 																			{
																				"box" : 																				{
																					"maxclass" : "message",
																					"text" : "utextcolor 0.51 0.44 0.49 $1",
																					"patching_rect" : [ 20.0, 140.0, 184.0, 21.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"numinlets" : 2,
																					"numoutlets" : 1,
																					"id" : "obj-9",
																					"outlettype" : [ "" ]
																				}

																			}
, 																			{
																				"box" : 																				{
																					"maxclass" : "newobj",
																					"text" : "p",
																					"patching_rect" : [ 340.0, 80.0, 52.0, 23.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
																					"numinlets" : 1,
																					"numoutlets" : 1,
																					"id" : "obj-3",
																					"outlettype" : [ "" ],
																					"patcher" : 																					{
																						"fileversion" : 1,
																						"appversion" : 																						{
																							"major" : 5,
																							"minor" : 1,
																							"revision" : 9
																						}
,
																						"rect" : [ 914.0, 177.0, 187.0, 216.0 ],
																						"bglocked" : 0,
																						"defrect" : [ 914.0, 177.0, 187.0, 216.0 ],
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
																						"boxes" : [ 																							{
																								"box" : 																								{
																									"maxclass" : "message",
																									"text" : "reload",
																									"patching_rect" : [ 80.0, 100.0, 49.0, 21.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-4",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "message",
																									"text" : "resave",
																									"patching_rect" : [ 20.0, 100.0, 53.0, 21.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-6",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "sel 115 -4",
																									"patching_rect" : [ 20.0, 60.0, 139.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 1,
																									"numoutlets" : 3,
																									"id" : "obj-7",
																									"outlettype" : [ "bang", "bang", "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "inlet",
																									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
																									"numinlets" : 0,
																									"numoutlets" : 1,
																									"id" : "obj-1",
																									"outlettype" : [ "int" ],
																									"comment" : ""
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "outlet",
																									"patching_rect" : [ 20.0, 160.0, 25.0, 25.0 ],
																									"numinlets" : 1,
																									"numoutlets" : 0,
																									"id" : "obj-2",
																									"comment" : ""
																								}

																							}
 ],
																						"lines" : [ 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-4", 0 ],
																									"destination" : [ "obj-2", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-6", 0 ],
																									"destination" : [ "obj-2", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-1", 0 ],
																									"destination" : [ "obj-7", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-7", 1 ],
																									"destination" : [ "obj-4", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-7", 0 ],
																									"destination" : [ "obj-6", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
 ]
																					}
,
																					"saved_object_attributes" : 																					{
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
, 																			{
																				"box" : 																				{
																					"maxclass" : "newobj",
																					"text" : "gate",
																					"patching_rect" : [ 240.0, 140.0, 59.0, 23.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"numinlets" : 2,
																					"numoutlets" : 1,
																					"id" : "obj-11",
																					"outlettype" : [ "" ]
																				}

																			}
, 																			{
																				"box" : 																				{
																					"maxclass" : "newobj",
																					"text" : "p",
																					"patching_rect" : [ 280.0, 80.0, 40.0, 23.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
																					"numinlets" : 2,
																					"numoutlets" : 1,
																					"id" : "obj-8",
																					"outlettype" : [ "" ],
																					"patcher" : 																					{
																						"fileversion" : 1,
																						"appversion" : 																						{
																							"major" : 5,
																							"minor" : 1,
																							"revision" : 9
																						}
,
																						"rect" : [ 791.0, 74.0, 355.0, 419.0 ],
																						"bglocked" : 0,
																						"defrect" : [ 791.0, 74.0, 355.0, 419.0 ],
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
																						"boxes" : [ 																							{
																								"box" : 																								{
																									"maxclass" : "outlet",
																									"patching_rect" : [ 20.0, 360.0, 25.0, 25.0 ],
																									"numinlets" : 1,
																									"numoutlets" : 0,
																									"id" : "obj-5",
																									"comment" : ""
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "loadmess 1",
																									"patching_rect" : [ 180.0, 100.0, 82.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 1,
																									"numoutlets" : 1,
																									"id" : "obj-2",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "zl join",
																									"patching_rect" : [ 20.0, 240.0, 59.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 2,
																									"id" : "obj-6",
																									"outlettype" : [ "", "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "coll",
																									"patching_rect" : [ 20.0, 320.0, 58.5, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
																									"numinlets" : 1,
																									"numoutlets" : 4,
																									"id" : "obj-1",
																									"outlettype" : [ "", "", "", "" ],
																									"coll_data" : 																									{
																										"count" : 30,
																										"data" : [ 																											{
																												"key" : 1049,
																												"value" : [ "value", 96 ]
																											}
, 																											{
																												"key" : 1050,
																												"value" : [ "value", 48 ]
																											}
, 																											{
																												"key" : 1051,
																												"value" : [ "value", 24 ]
																											}
, 																											{
																												"key" : 1052,
																												"value" : [ "value", 12 ]
																											}
, 																											{
																												"key" : 1053,
																												"value" : [ "value", 6 ]
																											}
, 																											{
																												"key" : 2049,
																												"value" : [ "value", 144 ]
																											}
, 																											{
																												"key" : 2050,
																												"value" : [ "value", 72 ]
																											}
, 																											{
																												"key" : 2051,
																												"value" : [ "value", 36 ]
																											}
, 																											{
																												"key" : 2052,
																												"value" : [ "value", 18 ]
																											}
, 																											{
																												"key" : 2053,
																												"value" : [ "value", 9 ]
																											}
, 																											{
																												"key" : 3049,
																												"value" : [ "value", 64 ]
																											}
, 																											{
																												"key" : 3050,
																												"value" : [ "value", 32 ]
																											}
, 																											{
																												"key" : 3051,
																												"value" : [ "value", 16 ]
																											}
, 																											{
																												"key" : 3052,
																												"value" : [ "value", 8 ]
																											}
, 																											{
																												"key" : 3053,
																												"value" : [ "value", 4 ]
																											}
, 																											{
																												"key" : 11049,
																												"value" : [ "cell", 96 ]
																											}
, 																											{
																												"key" : 11050,
																												"value" : [ "cell", 48 ]
																											}
, 																											{
																												"key" : 11051,
																												"value" : [ "cell", 24 ]
																											}
, 																											{
																												"key" : 11052,
																												"value" : [ "cell", 12 ]
																											}
, 																											{
																												"key" : 11053,
																												"value" : [ "cell", 6 ]
																											}
, 																											{
																												"key" : 12049,
																												"value" : [ "cell", 144 ]
																											}
, 																											{
																												"key" : 12050,
																												"value" : [ "cell", 72 ]
																											}
, 																											{
																												"key" : 12051,
																												"value" : [ "cell", 36 ]
																											}
, 																											{
																												"key" : 12052,
																												"value" : [ "cell", 18 ]
																											}
, 																											{
																												"key" : 12053,
																												"value" : [ "cell", 9 ]
																											}
, 																											{
																												"key" : 13049,
																												"value" : [ "cell", 64 ]
																											}
, 																											{
																												"key" : 13050,
																												"value" : [ "cell", 32 ]
																											}
, 																											{
																												"key" : 13051,
																												"value" : [ "cell", 16 ]
																											}
, 																											{
																												"key" : 13052,
																												"value" : [ "cell", 8 ]
																											}
, 																											{
																												"key" : 13053,
																												"value" : [ "cell", 4 ]
																											}
 ]
																									}
,
																									"saved_object_attributes" : 																									{
																										"embed" : 1
																									}

																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "expr (($i1 == 512) * 10000) + ($i2 * 1000) + $i3",
																									"patching_rect" : [ 20.0, 280.0, 302.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 3,
																									"numoutlets" : 1,
																									"id" : "obj-24",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "pak 1 0",
																									"patching_rect" : [ 60.0, 200.0, 106.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 1,
																									"id" : "obj-21",
																									"outlettype" : [ "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "sel 167",
																									"patching_rect" : [ 60.0, 100.0, 106.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 2,
																									"numoutlets" : 2,
																									"id" : "obj-17",
																									"outlettype" : [ "bang", "" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "newobj",
																									"text" : "counter 1 3",
																									"patching_rect" : [ 60.0, 160.0, 81.0, 23.0 ],
																									"fontname" : "Arial",
																									"fontsize" : 14.0,
																									"numinlets" : 5,
																									"numoutlets" : 4,
																									"id" : "obj-15",
																									"outlettype" : [ "int", "", "", "int" ]
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "inlet",
																									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
																									"numinlets" : 0,
																									"numoutlets" : 1,
																									"id" : "obj-3",
																									"outlettype" : [ "int" ],
																									"comment" : ""
																								}

																							}
, 																							{
																								"box" : 																								{
																									"maxclass" : "inlet",
																									"patching_rect" : [ 60.0, 20.0, 25.0, 25.0 ],
																									"numinlets" : 0,
																									"numoutlets" : 1,
																									"id" : "obj-4",
																									"outlettype" : [ "int" ],
																									"comment" : ""
																								}

																							}
 ],
																						"lines" : [ 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-21", 0 ],
																									"destination" : [ "obj-6", 1 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-17", 1 ],
																									"destination" : [ "obj-21", 1 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-15", 0 ],
																									"destination" : [ "obj-21", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-1", 0 ],
																									"destination" : [ "obj-5", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-4", 0 ],
																									"destination" : [ "obj-17", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-3", 0 ],
																									"destination" : [ "obj-6", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-2", 0 ],
																									"destination" : [ "obj-15", 3 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-17", 0 ],
																									"destination" : [ "obj-15", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-6", 0 ],
																									"destination" : [ "obj-24", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
, 																							{
																								"patchline" : 																								{
																									"source" : [ "obj-24", 0 ],
																									"destination" : [ "obj-1", 0 ],
																									"hidden" : 0,
																									"midpoints" : [  ]
																								}

																							}
 ]
																					}
,
																					"saved_object_attributes" : 																					{
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
, 																			{
																				"box" : 																				{
																					"maxclass" : "newobj",
																					"text" : "key",
																					"patching_rect" : [ 240.0, 20.0, 79.5, 23.0 ],
																					"fontname" : "Arial",
																					"fontsize" : 14.0,
																					"numinlets" : 0,
																					"numoutlets" : 4,
																					"id" : "obj-14",
																					"outlettype" : [ "int", "int", "int", "int" ]
																				}

																			}
, 																			{
																				"box" : 																				{
																					"maxclass" : "inlet",
																					"patching_rect" : [ 180.0, 20.0, 25.0, 25.0 ],
																					"numinlets" : 0,
																					"numoutlets" : 1,
																					"id" : "obj-12",
																					"outlettype" : [ "int" ],
																					"comment" : ""
																				}

																			}
, 																			{
																				"box" : 																				{
																					"maxclass" : "outlet",
																					"patching_rect" : [ 20.0, 280.0, 25.0, 25.0 ],
																					"numinlets" : 1,
																					"numoutlets" : 0,
																					"id" : "obj-13",
																					"comment" : ""
																				}

																			}
 ],
																		"lines" : [ 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-14", 2 ],
																					"destination" : [ "obj-8", 0 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-14", 3 ],
																					"destination" : [ "obj-8", 1 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-8", 0 ],
																					"destination" : [ "obj-11", 1 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-3", 0 ],
																					"destination" : [ "obj-11", 1 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-14", 3 ],
																					"destination" : [ "obj-3", 0 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-12", 0 ],
																					"destination" : [ "obj-9", 0 ],
																					"hidden" : 0,
																					"midpoints" : [ 189.5, 84.0, 29.5, 84.0 ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-12", 0 ],
																					"destination" : [ "obj-11", 0 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-11", 0 ],
																					"destination" : [ "obj-16", 1 ],
																					"hidden" : 0,
																					"midpoints" : [ 249.5, 179.0, 77.5, 179.0 ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-11", 0 ],
																					"destination" : [ "obj-13", 0 ],
																					"hidden" : 0,
																					"midpoints" : [ 249.5, 254.0, 29.5, 254.0 ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-9", 0 ],
																					"destination" : [ "obj-16", 0 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
, 																			{
																				"patchline" : 																				{
																					"source" : [ "obj-16", 0 ],
																					"destination" : [ "obj-13", 0 ],
																					"hidden" : 0,
																					"midpoints" : [  ]
																				}

																			}
 ]
																	}
,
																	"saved_object_attributes" : 																	{
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
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"patching_rect" : [ 200.0, 20.0, 25.0, 25.0 ],
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"id" : "obj-10",
																	"outlettype" : [ "int" ],
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "message",
																	"text" : "stop, forget, recall",
																	"patching_rect" : [ 60.0, 120.0, 122.0, 21.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"id" : "obj-5",
																	"outlettype" : [ "" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "sel reset",
																	"patching_rect" : [ 60.0, 80.0, 63.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 1,
																	"numoutlets" : 2,
																	"id" : "obj-3",
																	"outlettype" : [ "bang", "" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"patching_rect" : [ 20.0, 200.0, 25.0, 25.0 ],
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
																	"id" : "obj-1",
																	"outlettype" : [ "" ],
																	"comment" : ""
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-10", 0 ],
																	"destination" : [ "obj-15", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-15", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [ 209.5, 166.0, 29.5, 166.0 ]
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
																	"source" : [ "obj-1", 0 ],
																	"destination" : [ "obj-3", 0 ],
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
																	"source" : [ "obj-5", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [ 69.5, 166.0, 29.5, 166.0 ]
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
													"patching_rect" : [ 120.0, 160.0, 46.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-2",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 440.0, 280.0, 25.0, 25.0 ],
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
													"patching_rect" : [ 200.0, 160.0, 73.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-12",
													"outlettype" : [ "" ]
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
													"id" : "obj-9",
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "r",
													"patching_rect" : [ 300.0, 160.0, 33.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-4",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"patching_rect" : [ 300.0, 120.0, 49.0, 21.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-3",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "title $1",
													"patching_rect" : [ 360.0, 120.0, 52.0, 21.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-17",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "route title",
													"patching_rect" : [ 300.0, 60.0, 68.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 2,
													"id" : "obj-10",
													"outlettype" : [ "", "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 300.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-20",
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 360.0, 280.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-22",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-5", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 309.5, 204.0, 29.5, 204.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 209.5, 204.0, 29.5, 204.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 129.5, 204.0, 29.5, 204.0 ]
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
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [ 309.5, 97.0, 449.5, 97.0 ]
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
													"source" : [ "obj-17", 0 ],
													"destination" : [ "obj-22", 0 ],
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
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [ 309.5, 97.0, 369.5, 97.0 ]
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
									"patching_rect" : [ 120.0, 220.0, 79.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-9",
									"outlettype" : [ "", "" ],
									"save" : [ "#N", "thispatcher", ";", "#Q", "end", ";" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "quebec",
									"patching_rect" : [ 180.0, 140.0, 59.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-11",
									"outlettype" : [ "bang", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 1",
									"patching_rect" : [ 180.0, 100.0, 40.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"id" : "obj-8",
									"outlettype" : [ "bang", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "active",
									"patching_rect" : [ 120.0, 20.0, 47.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-2",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"hint" : "",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-1",
									"outlettype" : [ "" ],
									"comment" : "(anything) messages"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-23", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-23", 2 ],
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
									"source" : [ "obj-23", 1 ],
									"destination" : [ "obj-9", 0 ],
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
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
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
					"maxclass" : "message",
					"text" : ";\rtoto loop",
					"linecount" : 2,
					"patching_rect" : [ 16.0, 82.0, 64.0, 37.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"gradient" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-3",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"patching_rect" : [ 244.0, 232.0, 109.0, 94.0 ],
					"mode" : 0,
					"bordercolor" : [ 0.827451, 0.737255, 0.835294, 0.501961 ],
					"presentation" : 0,
					"border" : 2,
					"rounded" : 4,
					"background" : 0,
					"numinlets" : 1,
					"grad1" : [ 0.75, 0.79, 0.93, 1.0 ],
					"shadow" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"grad2" : [ 0.66, 0.66, 0.72, 1.0 ],
					"angle" : 0.0,
					"hidden" : 0,
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Last  ",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 13.0, 79.0, 30.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 3,
					"background" : 0,
					"fontsize" : 20.871338,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"underline" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"hidden" : 0,
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "December 28, 2012",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 45.0, 227.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"underline" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"hidden" : 0,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"patching_rect" : [ 10.0, 9.0, 355.0, 64.0 ],
					"mode" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"presentation" : 0,
					"border" : 0,
					"rounded" : 8,
					"background" : 1,
					"numinlets" : 1,
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"shadow" : 0,
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"angle" : 0.0,
					"hidden" : 0,
					"id" : "obj-23"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
