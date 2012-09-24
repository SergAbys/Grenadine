{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 305.0, 165.0, 364.0, 361.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 305.0, 165.0, 364.0, 361.0 ],
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
					"text" : "t 4",
					"patching_rect" : [ 120.0, 180.0, 26.0, 23.0 ],
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
					"hidden" : 0,
					"id" : "obj-5",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r matrix",
					"patching_rect" : [ 160.0, 180.0, 58.0, 23.0 ],
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
					"hidden" : 1,
					"id" : "obj-24",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"patching_rect" : [ 142.0, 218.0, 209.0, 126.0 ],
					"idlemouse" : 0,
					"presentation" : 0,
					"onscreen" : 1,
					"border" : 0.0,
					"depthbuffer" : 1,
					"usedstrect" : 0,
					"planemap" : [ 0, 1, 2, 3 ],
					"background" : 0,
					"srcrect" : [ 0, 0, 320, 240 ],
					"dstrect" : [ 0, 0, 320, 240 ],
					"numinlets" : 1,
					"fsaa" : 0,
					"interp" : 0,
					"usesrcrect" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"doublebuffer" : 1,
					"numoutlets" : 2,
					"stereo" : 0,
					"colormode" : "argb",
					"hidden" : 0,
					"id" : "obj-26",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r foxtrot",
					"patching_rect" : [ 220.0, 120.0, 58.0, 23.0 ],
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
					"hidden" : 1,
					"id" : "obj-2",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p dump",
					"patching_rect" : [ 220.0, 160.0, 57.0, 23.0 ],
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
					"numoutlets" : 0,
					"hidden" : 1,
					"id" : "obj-10",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 915.0, 110.0, 265.0, 489.0 ],
						"bglocked" : 0,
						"defrect" : [ 915.0, 110.0, 265.0, 489.0 ],
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
									"text" : "t b",
									"patching_rect" : [ 140.0, 80.0, 26.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-8",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"patching_rect" : [ 140.0, 120.0, 68.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-7",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "delay 1000",
									"patching_rect" : [ 140.0, 160.0, 79.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-6",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s clear",
									"patching_rect" : [ 80.0, 260.0, 52.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-9"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"patching_rect" : [ 20.0, 220.0, 79.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-5",
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r dump",
									"patching_rect" : [ 20.0, 20.0, 54.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-4",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s offset",
									"patching_rect" : [ 140.0, 440.0, 55.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s foxtrot",
									"patching_rect" : [ 20.0, 440.0, 60.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 140.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-1",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend dump",
									"patching_rect" : [ 20.0, 400.0, 101.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-40",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t i i",
									"patching_rect" : [ 20.0, 340.0, 104.5, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-39",
									"outlettype" : [ "int", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pak offset 0 0",
									"patching_rect" : [ 140.0, 400.0, 94.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 3,
									"numoutlets" : 1,
									"id" : "obj-37",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 1",
									"patching_rect" : [ 20.0, 300.0, 32.5, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-34",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 6",
									"patching_rect" : [ 20.0, 260.0, 46.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 3,
									"id" : "obj-33",
									"outlettype" : [ "bang", "bang", "int" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-39", 1 ],
									"destination" : [ "obj-37", 2 ],
									"hidden" : 0,
									"midpoints" : [ 115.0, 381.0, 224.5, 381.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [ 149.5, 207.0, 29.5, 207.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 1 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-39", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-39", 0 ],
									"destination" : [ "obj-40", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 2 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-33", 0 ],
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
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 80.0, 320.0, 32.0, 23.0 ],
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
					"numoutlets" : 0,
					"hidden" : 0,
					"id" : "obj-15",
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 103.0, 117.0, 241.0, 372.0 ],
						"bglocked" : 0,
						"defrect" : [ 103.0, 117.0, 241.0, 372.0 ],
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
									"text" : "route emissions",
									"patching_rect" : [ 20.0, 60.0, 109.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-1",
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-7",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b clear",
									"patching_rect" : [ 120.0, 180.0, 60.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-6",
									"outlettype" : [ "bang", "clear" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r clear",
									"patching_rect" : [ 120.0, 140.0, 49.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-5",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s matrix",
									"patching_rect" : [ 20.0, 320.0, 59.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r offset",
									"patching_rect" : [ 40.0, 140.0, 53.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-2",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "vexpr $i1 * 25 @scalarmode 1",
									"patching_rect" : [ 20.0, 100.0, 198.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-43",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.fill foo 1",
									"patching_rect" : [ 20.0, 180.0, 73.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-30",
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.matrix foo 4 char 127 6",
									"patching_rect" : [ 20.0, 280.0, 168.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-29",
									"outlettype" : [ "jit_matrix", "" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [ 170.5, 231.0, 29.5, 231.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-43", 0 ],
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
					"maxclass" : "toggle",
					"patching_rect" : [ 120.0, 100.0, 20.0, 20.0 ],
					"presentation" : 0,
					"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"background" : 0,
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-21",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "metro 250",
					"patching_rect" : [ 120.0, 140.0, 74.0, 23.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"numinlets" : 2,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-19",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"patching_rect" : [ 20.0, 320.0, 52.0, 23.0 ],
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
					"numoutlets" : 0,
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
						"rect" : [ 417.0, 141.0, 368.0, 385.0 ],
						"bglocked" : 0,
						"defrect" : [ 417.0, 141.0, 368.0, 385.0 ],
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
									"patching_rect" : [ 20.0, 60.0, 32.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-3",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 364.0, 91.0, 336.0, 149.0 ],
										"bglocked" : 0,
										"defrect" : [ 364.0, 91.0, 336.0, 149.0 ],
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
													"text" : "vexpr $i1*(random (0\\,7) != 0) @scalarmode 1",
													"patching_rect" : [ 20.0, 60.0, 296.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-3",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 20.0, 100.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
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
													"id" : "obj-1",
													"outlettype" : [ "list" ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
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
									"maxclass" : "newobj",
									"text" : "freebang",
									"patching_rect" : [ 300.0, 40.0, 66.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 0,
									"numoutlets" : 1,
									"hidden" : 1,
									"id" : "obj-10",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"patching_rect" : [ 300.0, 120.0, 36.0, 23.0 ],
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
									"patching_rect" : [ 300.0, 80.0, 72.0, 23.0 ],
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
									"patching_rect" : [ 300.0, 160.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 3,
									"numoutlets" : 0,
									"hidden" : 1,
									"id" : "obj-19"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 26",
									"patching_rect" : [ 240.0, 280.0, 90.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"hidden" : 1,
									"id" : "obj-2",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p Y",
									"patching_rect" : [ 200.0, 280.0, 31.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-5",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 495.0, 187.0, 132.0, 419.0 ],
										"bglocked" : 0,
										"defrect" : [ 495.0, 187.0, 132.0, 419.0 ],
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
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-9",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 20.0, 360.0, 25.0, 25.0 ],
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
													"patching_rect" : [ 20.0, 320.0, 40.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-6",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"patching_rect" : [ 20.0, 220.0, 40.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 2,
													"id" : "obj-5",
													"outlettype" : [ "bang", "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "uzi 16",
													"patching_rect" : [ 20.0, 140.0, 48.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 3,
													"id" : "obj-4",
													"outlettype" : [ "bang", "bang", "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int",
													"patching_rect" : [ 60.0, 280.0, 32.5, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-3",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"patching_rect" : [ 20.0, 100.0, 73.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 2,
													"id" : "obj-2",
													"outlettype" : [ "bang", "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-1",
													"outlettype" : [ "int" ],
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
													"source" : [ "obj-4", 2 ],
													"destination" : [ "obj-5", 0 ],
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
													"source" : [ "obj-5", 1 ],
													"destination" : [ "obj-6", 1 ],
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
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-6", 0 ],
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
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-2", 0 ],
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
									"maxclass" : "umenu",
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"patching_rect" : [ 200.0, 220.0, 151.0, 25.0 ],
									"fontname" : "Arial",
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"arrow" : 0,
									"types" : [  ],
									"fontsize" : 16.0,
									"numinlets" : 1,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
									"align" : 1,
									"arrowframe" : 0,
									"numoutlets" : 3,
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"id" : "obj-6",
									"outlettype" : [ "int", "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout",
									"patching_rect" : [ 200.0, 340.0, 57.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 0,
									"id" : "obj-368"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"patching_rect" : [ 20.0, 260.0, 32.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-16",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 614.0, 149.0, 251.0, 309.0 ],
										"bglocked" : 0,
										"defrect" : [ 614.0, 149.0, 251.0, 309.0 ],
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
													"text" : "sel 0",
													"patching_rect" : [ 20.0, 60.0, 40.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 2,
													"id" : "obj-4",
													"outlettype" : [ "bang", "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p",
													"patching_rect" : [ 180.0, 160.0, 34.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-38",
													"outlettype" : [ "float" ],
													"patcher" : 													{
														"fileversion" : 1,
														"appversion" : 														{
															"major" : 5,
															"minor" : 1,
															"revision" : 9
														}
,
														"rect" : [ 50.0, 180.0, 109.0, 266.0 ],
														"bglocked" : 0,
														"defrect" : [ 50.0, 180.0, 109.0, 266.0 ],
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
																	"text" : "* 1000.",
																	"patching_rect" : [ 20.0, 180.0, 55.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"id" : "obj-27",
																	"outlettype" : [ "float" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "!- 1.",
																	"patching_rect" : [ 20.0, 100.0, 35.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"id" : "obj-26",
																	"outlettype" : [ "float" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "pow 2",
																	"patching_rect" : [ 20.0, 140.0, 48.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"id" : "obj-21",
																	"outlettype" : [ "float" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "/ 127.",
																	"patching_rect" : [ 20.0, 60.0, 46.0, 23.0 ],
																	"fontname" : "Arial",
																	"fontsize" : 14.0,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"id" : "obj-7",
																	"outlettype" : [ "float" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"id" : "obj-35",
																	"outlettype" : [ "int" ],
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"patching_rect" : [ 20.0, 220.0, 25.0, 25.0 ],
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"id" : "obj-37",
																	"comment" : ""
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-26", 0 ],
																	"destination" : [ "obj-21", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-35", 0 ],
																	"destination" : [ "obj-7", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-7", 0 ],
																	"destination" : [ "obj-26", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-21", 0 ],
																	"destination" : [ "obj-27", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-27", 0 ],
																	"destination" : [ "obj-37", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
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
													"text" : "+ 30",
													"patching_rect" : [ 80.0, 180.0, 38.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-1",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "random 50",
													"patching_rect" : [ 80.0, 140.0, 77.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-3",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0 0 0",
													"patching_rect" : [ 20.0, 220.0, 138.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 3,
													"numoutlets" : 1,
													"id" : "obj-5",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t i b i",
													"patching_rect" : [ 20.0, 100.0, 139.0, 23.0 ],
													"fontname" : "Arial",
													"fontsize" : 14.0,
													"numinlets" : 1,
													"numoutlets" : 3,
													"id" : "obj-2",
													"outlettype" : [ "int", "bang", "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"id" : "obj-14",
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"patching_rect" : [ 20.0, 260.0, 25.0, 25.0 ],
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-15",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-15", 0 ],
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
													"source" : [ "obj-2", 1 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-5", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 2 ],
													"destination" : [ "obj-38", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-38", 0 ],
													"destination" : [ "obj-5", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 1 ],
													"destination" : [ "obj-2", 0 ],
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
									"text" : "makenote 90 250 1",
									"patching_rect" : [ 20.0, 300.0, 138.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 4,
									"numoutlets" : 3,
									"id" : "obj-8",
									"outlettype" : [ "float", "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pipe 775",
									"patching_rect" : [ 80.0, 160.0, 64.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-13",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pipe 375",
									"patching_rect" : [ 200.0, 160.0, 64.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-7",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0 0 0 0",
									"patching_rect" : [ 20.0, 120.0, 199.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 4,
									"id" : "obj-9",
									"outlettype" : [ "int", "int", "int", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"patching_rect" : [ 20.0, 340.0, 138.0, 23.0 ],
									"fontname" : "Arial",
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 3,
									"numoutlets" : 0,
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"id" : "obj-1",
									"outlettype" : [ "list" ],
									"comment" : "(list)"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [ 209.5, 210.0, 29.5, 210.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 2 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [ 149.5, 210.0, 29.5, 210.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-368", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 1 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 3 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 2 ],
									"destination" : [ "obj-4", 2 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 1 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-8", 0 ],
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
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
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
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-9", 0 ],
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
					"text" : "clear",
					"patching_rect" : [ 60.0, 180.0, 41.0, 21.0 ],
					"fontname" : "Arial",
					"presentation" : 0,
					"gradient" : 0,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"fontface" : 0,
					"background" : 0,
					"fontsize" : 14.0,
					"numinlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-12",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 20.0, 100.0, 22.0, 22.0 ],
					"presentation" : 0,
					"fgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"outlinecolor" : [ 0.71328, 0.71328, 0.71328, 1.0 ],
					"background" : 0,
					"numinlets" : 1,
					"blinkcolor" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"bgcolor" : [ 0.91328, 0.91328, 0.91328, 0.75 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-3",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p TUTO41",
					"patching_rect" : [ 20.0, 140.0, 76.0, 23.0 ],
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
					"numoutlets" : 1,
					"hidden" : 0,
					"id" : "obj-25",
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 540.0, 73.0, 432.0, 603.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 540.0, 73.0, 432.0, 603.0 ],
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
						"imprint" : 1,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s dump",
									"patching_rect" : [ 60.0, 520.0, 56.0, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 1,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 0,
									"hidden" : 0,
									"id" : "obj-12"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b l",
									"patching_rect" : [ 60.0, 460.0, 59.0, 23.0 ],
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
									"numoutlets" : 2,
									"hidden" : 0,
									"id" : "obj-5",
									"outlettype" : [ "bang", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend learn",
									"patching_rect" : [ 140.0, 520.0, 97.0, 23.0 ],
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
									"hidden" : 0,
									"id" : "obj-4",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl group 6",
									"patching_rect" : [ 60.0, 420.0, 72.0, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"numinlets" : 2,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 2,
									"hidden" : 0,
									"id" : "obj-3",
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "if $i1>=0 then $i1",
									"patching_rect" : [ 80.0, 220.0, 118.0, 23.0 ],
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
									"hidden" : 0,
									"id" : "obj-8",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"patching_rect" : [ 140.0, 560.0, 25.0, 25.0 ],
									"presentation" : 0,
									"background" : 0,
									"numinlets" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 0,
									"hidden" : 0,
									"id" : "obj-11",
									"comment" : "(int)"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "trigger next",
									"patching_rect" : [ 80.0, 340.0, 81.0, 23.0 ],
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
									"hidden" : 0,
									"id" : "obj-10",
									"outlettype" : [ "next" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"patching_rect" : [ 120.0, 20.0, 68.0, 23.0 ],
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
									"hidden" : 0,
									"id" : "obj-9",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t next start",
									"patching_rect" : [ 20.0, 60.0, 76.0, 23.0 ],
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
									"numoutlets" : 2,
									"hidden" : 0,
									"id" : "obj-7",
									"outlettype" : [ "next", "start" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"presentation" : 0,
									"background" : 0,
									"numinlets" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 1,
									"hidden" : 0,
									"id" : "obj-2",
									"outlettype" : [ "bang" ],
									"comment" : "(bang)"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "delay",
									"patching_rect" : [ 80.0, 300.0, 44.0, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"numinlets" : 2,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 1,
									"hidden" : 0,
									"id" : "obj-20",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "trigger b i",
									"patching_rect" : [ 80.0, 260.0, 69.0, 23.0 ],
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
									"numoutlets" : 2,
									"hidden" : 0,
									"id" : "obj-19",
									"outlettype" : [ "bang", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"patching_rect" : [ 220.0, 260.0, 130.0, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"numinlets" : 3,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 0,
									"hidden" : 0,
									"id" : "obj-17"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 90 250 1",
									"patching_rect" : [ 220.0, 220.0, 130.0, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"numinlets" : 4,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 3,
									"hidden" : 0,
									"id" : "obj-16",
									"outlettype" : [ "float", "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "read tutorial41.mid",
									"patching_rect" : [ 120.0, 60.0, 126.0, 21.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"gradient" : 0,
									"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"numinlets" : 2,
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 1,
									"hidden" : 0,
									"id" : "obj-6",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "detonate",
									"patching_rect" : [ 20.0, 140.0, 300.5, 23.0 ],
									"fontname" : "Arial",
									"presentation" : 0,
									"fontface" : 0,
									"background" : 0,
									"fontsize" : 14.0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"numinlets" : 8,
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"numoutlets" : 8,
									"hidden" : 0,
									"id" : "obj-1",
									"outlettype" : [ "int", "int", "int", "int", "int", "int", "int", "int" ],
									"save" : [ "#N", "detonate", "", ";", "#X", "setparam", 0, "Time", 0, 0, 999999, 0, 1000, 200, 0, ";", "#X", "setparam", 1, "Pitch", 0, 0, 127, 60, 12, 4, 0, ";", "#X", "setparam", 2, "Vel", 0, 0, 127, 64, 12, 4, 0, ";", "#X", "setparam", 3, "Dur", 0, 1, 99999, 200, 800, 200, 0, ";", "#X", "setparam", 4, "Chan", 0, 1, 16, 1, 8, 1, 0, ";", "#X", "setparam", 5, "Track", 0, 1, 32, 1, 8, 1, 0, ";", "#X", "setparam", 6, "X1", 0, 0, 999, 0, 80, 20, 0, ";", "#X", "setparam", 7, "X2", 0, 0, 999, 0, 80, 20, 0, ";", "#X", "restore", ";", "#X", 0, 40, 54, 994, 4, 1, 0, 0, ";", "#X", 0, 71, 85, 57, 4, 1, 0, 0, ";", "#X", 57, 72, 62, 62, 4, 1, 0, 0, ";", "#X", 62, 47, 55, 10, 4, 1, 0, 0, ";", "#X", 0, 71, 87, 63, 4, 1, 0, 0, ";", "#X", 63, 69, 103, 62, 4, 1, 0, 0, ";", "#X", 62, 52, 63, 10, 4, 1, 0, 0, ";", "#X", 0, 71, 96, 10, 4, 1, 0, 0, ";", "#X", 125, 59, 64, 125, 4, 1, 0, 0, ";", "#X", 125, 64, 64, 10, 4, 1, 0, 0, ";", "#X", 84, 72, 71, 10, 4, 1, 0, 0, ";", "#X", 41, 65, 54, 10, 4, 1, 0, 0, ";", "#X", 42, 74, 77, 83, 4, 1, 0, 0, ";", "#X", 83, 64, 64, 10, 4, 1, 0, 0, ";", "#X", 0, 76, 84, 84, 4, 1, 0, 0, ";", "#X", 84, 74, 70, 10, 4, 1, 0, 0, ";", "#X", 41, 59, 57, 10, 4, 1, 0, 0, ";", "#X", 42, 72, 73, 83, 4, 1, 0, 0, ";", "#X", 83, 62, 70, 500, 4, 1, 0, 0, ";", "#X", 0, 71, 62, 84, 4, 1, 0, 0, ";", "#X", 84, 69, 64, 10, 4, 1, 0, 0, ";", "#X", 41, 38, 57, 10, 4, 1, 0, 0, ";", "#X", 42, 67, 66, 83, 4, 1, 0, 0, ";", "#X", 83, 50, 71, 10, 4, 1, 0, 0, ";", "#X", 0, 69, 96, 10, 4, 1, 0, 0, ";", "#X", 125, 57, 69, 125, 4, 1, 0, 0, ";", "#X", 125, 60, 60, 125, 4, 1, 0, 0, ";", "#X", 84, 71, 62, 83, 4, 1, 0, 0, ";", "#X", 41, 62, 59, 10, 4, 1, 0, 0, ";", "#X", 42, 69, 70, 83, 4, 1, 0, 0, ";", "#X", 83, 60, 60, 125, 4, 1, 0, 0, ";", "#X", 0, 67, 84, 84, 4, 1, 0, 0, ";", "#X", 84, 69, 74, 83, 4, 1, 0, 0, ";", "#X", 41, 59, 75, 10, 4, 1, 0, 0, ";", "#X", 42, 67, 88, 83, 4, 1, 0, 0, ";", "#X", 83, 57, 82, 375, 4, 1, 0, 0, ";", "#X", 0, 65, 66, 63, 4, 1, 0, 0, ";", "#X", 63, 67, 56, 62, 4, 1, 0, 0, ";", "#X", 62, 45, 63, 10, 4, 1, 0, 0, ";", "#X", 0, 65, 67, 63, 4, 1, 0, 0, ";", "#X", 63, 64, 80, 10, 4, 1, 0, 0, ";", "#X", 62, 52, 66, 125, 4, 1, 0, 0, ";", "#X", 125, 71, 63, 125, 4, 1, 0, 0, ";", "#X", 0, 57, 66, 125, 4, 1, 0, 0, ";", "#X", 125, 76, 68, 63, 4, 1, 0, 0, ";", "#X", 0, 55, 72, 375, 4, 1, 0, 0, ";", "#X", 63, 77, 63, 10, 4, 1, 0, 0, ";", "#X", 62, 43, 56, 10, 4, 1, 0, 0, ";", "#X", 0, 76, 82, 10, 4, 1, 0, 0, ";", "#X", 63, 74, 80, 10, 4, 1, 0, 0, ";", "#X", 62, 50, 58, 10, 4, 1, 0, 0, ";", "#X", 125, 55, 63, 125, 4, 1, 0, 0, ";", "#X", 125, 72, 84, 167, 4, 1, 0, 0, ";", "#X", 0, 53, 83, 10, 4, 1, 0, 0, ";", "#X", 125, 41, 67, 875, 4, 1, 0, 0, ";", "#X", 42, 71, 89, 10, 4, 1, 0, 0, ";", "#X", 83, 48, 66, 10, 4, 1, 0, 0, ";", "#X", 84, 69, 100, 10, 4, 1, 0, 0, ";", "#X", 41, 53, 74, 10, 4, 1, 0, 0, ";", "#X", 125, 60, 63, 10, 4, 1, 0, 0, ";", "#X", 125, 69, 70, 10, 4, 1, 0, 0, ";", "#X", 125, 67, 61, 10, 4, 1, 0, 0, ";", "#X", 125, 65, 59, 10, 4, 1, 0, 0, ";", "#X", "stop", ";" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 1 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 1 ],
									"destination" : [ "obj-20", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 1 ],
									"destination" : [ "obj-17", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 2 ],
									"destination" : [ "obj-17", 2 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 69.714287, 191.0, 229.5, 191.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-16", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 109.928574, 191.0, 266.5, 191.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 3 ],
									"destination" : [ "obj-16", 2 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 150.142853, 191.0, 303.5, 191.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 4 ],
									"destination" : [ "obj-16", 3 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 190.357147, 191.0, 340.5, 191.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 89.5, 385.0, 395.0, 385.0, 395.0, 122.0, 29.5, 122.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 1 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
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
					"text" : "foxtrot",
					"patching_rect" : [ 20.0, 260.0, 49.0, 23.0 ],
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
					"numoutlets" : 2,
					"hidden" : 0,
					"id" : "obj-1",
					"outlettype" : [ "list", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Hidden Markov Model // 32 32",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 43.0, 230.0, 23.0 ],
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
					"maxclass" : "comment",
					"text" : "Foxtrot",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 13.0, 332.0, 30.0 ],
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
					"maxclass" : "panel",
					"patching_rect" : [ 10.0, 9.0, 344.0, 68.0 ],
					"mode" : 1,
					"presentation" : 0,
					"angle" : 0.0,
					"border" : 0,
					"rounded" : 8,
					"background" : 1,
					"numinlets" : 1,
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"shadow" : 0,
					"hidden" : 0,
					"id" : "obj-23"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 129.5, 214.0, 29.5, 214.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
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
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
