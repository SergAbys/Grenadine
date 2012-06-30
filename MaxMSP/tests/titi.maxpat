{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 107.0, 114.0, 317.0, 310.0 ],
		"bglocked" : 0,
		"defrect" : [ 107.0, 114.0, 317.0, 310.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "#####",
					"patching_rect" : [ 220.0, 100.0, 49.0, 21.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 14.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-10"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 220.0, 160.0, 38.0, 23.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "end u805000106",
					"patching_rect" : [ 120.0, 240.0, 119.0, 21.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 14.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"patching_rect" : [ 160.0, 60.0, 52.0, 23.0 ],
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 0,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-7",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 372.0, 216.0, 267.0, 216.0 ],
						"bglocked" : 0,
						"defrect" : [ 372.0, 216.0, 267.0, 216.0 ],
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
									"text" : "p",
									"patching_rect" : [ 20.0, 120.0, 22.0, 23.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-6",
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 495.0, 187.0, 125.0, 343.0 ],
										"bglocked" : 0,
										"defrect" : [ 495.0, 187.0, 125.0, 343.0 ],
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
													"text" : "+ 1",
													"patching_rect" : [ 20.0, 60.0, 32.5, 23.0 ],
													"outlettype" : [ "int" ],
													"fontsize" : 14.0,
													"numinlets" : 2,
													"fontname" : "Arial",
													"numoutlets" : 1,
													"id" : "obj-9"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 20.0, 300.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-7",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack",
													"patching_rect" : [ 20.0, 260.0, 40.0, 23.0 ],
													"outlettype" : [ "" ],
													"fontsize" : 14.0,
													"numinlets" : 2,
													"fontname" : "Arial",
													"numoutlets" : 1,
													"id" : "obj-6"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"patching_rect" : [ 20.0, 180.0, 40.0, 23.0 ],
													"outlettype" : [ "bang", "int" ],
													"fontsize" : 14.0,
													"numinlets" : 1,
													"fontname" : "Arial",
													"numoutlets" : 2,
													"id" : "obj-5"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "uzi 16",
													"patching_rect" : [ 20.0, 140.0, 48.0, 23.0 ],
													"outlettype" : [ "bang", "bang", "int" ],
													"fontsize" : 14.0,
													"numinlets" : 2,
													"fontname" : "Arial",
													"numoutlets" : 3,
													"id" : "obj-4"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int",
													"patching_rect" : [ 80.0, 220.0, 32.5, 23.0 ],
													"outlettype" : [ "int" ],
													"fontsize" : 14.0,
													"numinlets" : 2,
													"fontname" : "Arial",
													"numoutlets" : 1,
													"id" : "obj-3"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"patching_rect" : [ 20.0, 100.0, 54.0, 23.0 ],
													"outlettype" : [ "bang", "int" ],
													"fontsize" : 14.0,
													"numinlets" : 1,
													"fontname" : "Arial",
													"numoutlets" : 2,
													"id" : "obj-2"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"outlettype" : [ "int" ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-1",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-6", 0 ],
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
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 1 ],
													"destination" : [ "obj-6", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 1 ],
													"destination" : [ "obj-3", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 2 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontface" : 0,
										"globalpatchername" : "",
										"fontsize" : 14.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 14.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 4",
									"patching_rect" : [ 60.0, 80.0, 82.0, 23.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 14.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-10"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"patching_rect" : [ 20.0, 40.0, 116.0, 25.0 ],
									"outlettype" : [ "int", "", "" ],
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"fontsize" : 16.0,
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"arrow" : 0,
									"align" : 1,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"arrowframe" : 0,
									"numinlets" : 1,
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-11",
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"types" : [  ],
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout",
									"patching_rect" : [ 20.0, 160.0, 57.0, 23.0 ],
									"fontsize" : 14.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 0,
									"id" : "obj-12"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "freebang",
									"patching_rect" : [ 160.0, 40.0, 66.0, 23.0 ],
									"outlettype" : [ "bang" ],
									"fontsize" : 14.0,
									"numinlets" : 0,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-21"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"patching_rect" : [ 160.0, 120.0, 36.0, 23.0 ],
									"outlettype" : [ "int" ],
									"fontsize" : 14.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-18"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 16 1",
									"patching_rect" : [ 160.0, 80.0, 72.0, 23.0 ],
									"outlettype" : [ "bang", "bang", "int" ],
									"fontsize" : 14.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-17"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ctlout 123",
									"patching_rect" : [ 160.0, 160.0, 72.0, 23.0 ],
									"fontsize" : 14.0,
									"numinlets" : 3,
									"fontname" : "Arial",
									"numoutlets" : 0,
									"id" : "obj-19"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 2 ],
									"destination" : [ "obj-19", 2 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"globalpatchername" : "",
						"fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tralala",
					"patching_rect" : [ 60.0, 100.0, 54.5, 23.0 ],
					"outlettype" : [ "list", "", "", "bang" ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 4,
					"id" : "obj-1",
					"tralala" : 					{
						"restore" : 						{
							"bpm" : [ "bpm", 240 ],
							"chance" : [ "chance", 100 ],
							"velocity" : [ "velocity", 0 ],
							"channel" : [ "channel", 1 ],
							"chord" : [ "chord", 0 ],
							"cell" : [ "cell", 12 ],
							"value" : [ "value", 12 ],
							"scale" : [ "scale", "C", "none" ],
							"pattern" : [ "pattern" ],
							"zone" : [ "zone", 0, 96, 60, 71 ],
							"0" : [ "note", 36, 62, 80, 12, 0 ],
							"1" : [ "note", 84, 66, 80, 12, 0 ],
							"2" : [ "note", 48, 64, 80, 12, 0 ],
							"3" : [ "note", 60, 64, 80, 12, 0 ],
							"4" : [ "note", 24, 62, 80, 12, 0 ],
							"5" : [ "note", 12, 60, 80, 12, 0 ],
							"6" : [ "note", 0, 60, 80, 12, 0 ],
							"7" : [ "note", 72, 66, 80, 12, 0 ]
						}
,
						"current" : 						{
							"bpm" : [ "bpm", 240 ],
							"chance" : [ "chance", 100 ],
							"velocity" : [ "velocity", 0 ],
							"channel" : [ "channel", 1 ],
							"chord" : [ "chord", 0 ],
							"cell" : [ "cell", 12 ],
							"value" : [ "value", 12 ],
							"scale" : [ "scale", "C", "none" ],
							"pattern" : [ "pattern" ],
							"zone" : [ "zone", 0, 96, 60, 71 ],
							"0" : [ "note", 36, 62, 80, 12, 0 ],
							"1" : [ "note", 84, 66, 80, 12, 0 ],
							"2" : [ "note", 48, 64, 80, 12, 0 ],
							"3" : [ "note", 60, 64, 80, 12, 0 ],
							"4" : [ "note", 24, 62, 80, 12, 0 ],
							"5" : [ "note", 12, 60, 80, 12, 0 ],
							"6" : [ "note", 0, 60, 80, 12, 0 ],
							"7" : [ "note", 72, 66, 80, 12, 0 ]
						}

					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 40.0, 140.0, 24.0, 23.0 ],
					"fontsize" : 14.0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-14",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 416.0, 128.0, 363.0, 215.0 ],
						"bglocked" : 0,
						"defrect" : [ 416.0, 128.0, 363.0, 215.0 ],
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
									"text" : "noteout",
									"patching_rect" : [ 50.0, 140.0, 254.0, 23.0 ],
									"fontsize" : 14.0,
									"numinlets" : 3,
									"fontname" : "Arial",
									"numoutlets" : 0,
									"id" : "obj-10"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 60 90 250 1 @repeatmode 1",
									"patching_rect" : [ 50.0, 100.0, 254.0, 23.0 ],
									"outlettype" : [ "float", "float", "float" ],
									"fontsize" : 14.0,
									"numinlets" : 4,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-9"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
									"outlettype" : [ "list" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-13",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 2 ],
									"destination" : [ "obj-10", 2 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 1 ],
									"destination" : [ "obj-10", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"globalpatchername" : "",
						"fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 14.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 40.0, 40.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"patching_rect" : [ 120.0, 20.0, 37.0, 21.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 14.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-8"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
