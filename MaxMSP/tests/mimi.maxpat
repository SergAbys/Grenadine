{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 288.0, 114.0, 589.0, 459.0 ],
		"bglocked" : 0,
		"defrect" : [ 288.0, 114.0, 589.0, 459.0 ],
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
					"text" : "store",
					"patching_rect" : [ 200.0, 40.0, 42.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-5",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tralala",
					"patching_rect" : [ 20.0, 140.0, 258.0, 190.0 ],
					"lassocolor" : [ 0.827451, 0.827451, 0.827451, 0.345098 ],
					"yoffset" : 633,
					"hcolor1" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
					"ubordercolor" : [ 0.827451, 0.827451, 0.827451, 1.0 ],
					"color" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"numinlets" : 1,
					"hcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"xoffset" : -41,
					"textcolor" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
					"numoutlets" : 4,
					"utextcolor" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"id" : "obj-1",
					"outlettype" : [ "list", "", "", "bang" ],
					"tralala" : 					{
						"untitled" : 						{
							"bpm" : [ "bpm", 120 ],
							"chance" : [ "chance", 100 ],
							"velocity" : [ "velocity", 0 ],
							"channel" : [ "channel", 1 ],
							"chord" : [ "chord", 0 ],
							"cell" : [ "cell", 12 ],
							"value" : [ "value", 12 ],
							"scale" : [ "scale", "C", "none" ],
							"pattern" : [ "pattern" ],
							"zone" : [ "zone", 0, 96, 60, 71 ],
							"0" : [ "note", 12, 70, 80, 12, 0 ],
							"1" : [ "note", 24, 69, 80, 12, 0 ],
							"2" : [ "note", 36, 68, 80, 12, 0 ],
							"3" : [ "note", 48, 67, 80, 12, 0 ],
							"4" : [ "note", 60, 68, 80, 12, 0 ],
							"5" : [ "note", 72, 69, 80, 12, 0 ],
							"6" : [ "note", 84, 70, 80, 12, 0 ],
							"7" : [ "note", 0, 69, 80, 12, 0 ]
						}

					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 20.0, 340.0, 47.0, 23.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-4",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 25.0, 69.0, 214.0, 281.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 214.0, 281.0 ],
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
									"maxclass" : "flonum",
									"patching_rect" : [ 20.0, 160.0, 74.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-11",
									"outlettype" : [ "float", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print !!!",
									"patching_rect" : [ 100.0, 240.0, 53.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-7"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "split 124. 126.",
									"patching_rect" : [ 20.0, 200.0, 99.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 3,
									"numoutlets" : 2,
									"id" : "obj-6",
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"patching_rect" : [ 20.0, 80.0, 38.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-21",
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p cputimer",
									"patching_rect" : [ 20.0, 120.0, 77.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-20",
									"outlettype" : [ "float" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 525.0, 182.0, 197.0, 237.0 ],
										"bglocked" : 0,
										"defrect" : [ 525.0, 182.0, 197.0, 237.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 11.595187,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 15.0, 15.0 ],
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
													"patching_rect" : [ 34.0, 173.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-1",
													"comment" : "interval in ms is reported here"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "b 1",
													"patching_rect" : [ 107.0, 78.0, 27.0, 20.0 ],
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-2",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "cpuclock",
													"patching_rect" : [ 107.0, 104.0, 56.0, 20.0 ],
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-3",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "- 0.",
													"patching_rect" : [ 34.0, 143.0, 32.5, 20.0 ],
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-4",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "b 1",
													"patching_rect" : [ 48.0, 78.0, 27.0, 20.0 ],
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-5",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "cpuclock",
													"patching_rect" : [ 47.5, 104.0, 56.0, 20.0 ],
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-6",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 107.0, 47.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-7",
													"outlettype" : [ "bang" ],
													"comment" : "bang stops timing and reports interval"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 48.0, 47.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-8",
													"outlettype" : [ "bang" ],
													"comment" : "bang starts timing interval"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-4", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-1", 0 ],
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
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-3", 0 ],
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
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-5", 0 ],
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
										"fontsize" : 11.595187,
										"default_fontsize" : 11.595187
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-2",
									"outlettype" : [ "list" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 1 ],
									"destination" : [ "obj-20", 1 ],
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
					"text" : "load",
					"patching_rect" : [ 120.0, 20.0, 37.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"patching_rect" : [ 60.0, 40.0, 37.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-15",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "loop",
					"patching_rect" : [ 20.0, 40.0, 37.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-8",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "scramble",
					"patching_rect" : [ 100.0, 80.0, 67.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-9",
					"outlettype" : [ "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
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
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
