{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 776.0, 110.0, 408.0, 413.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 776.0, 110.0, 408.0, 413.0 ],
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
					"text" : "loadmess 0.5",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 98.0, 297.0, 94.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 50.0, 174.0, 52.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 1,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 25.0, 69.0, 422.0, 250.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 422.0, 250.0 ],
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
									"text" : "t \\\\",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 200.0, 120.0, 26.0, 23.0 ],
									"outlettype" : [ "\\" ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "click and wait ...",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 120.0, 109.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 1 0",
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"patching_rect" : [ 20.0, 60.0, 379.0, 23.0 ],
									"outlettype" : [ "bang", "bang", "" ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-31",
									"numinlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 200.0, 25.0, 25.0 ],
									"id" : "obj-32",
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-32", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-32", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontface" : 0,
					"fontsize" : 18.0,
					"textcolor" : [ 0.470588, 0.333333, 0.482353, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 46.0, 99.0, 142.0, 25.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-24",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"settype" : 1,
					"numoutlets" : 2,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 220.0, 227.0, 166.0, 171.0 ],
					"border_right" : 0,
					"bgcolor" : [ 0.858824, 0.858824, 0.858824, 0.0 ],
					"candicane5" : [ 0.58784, 0.82881, 0.43166, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "", "" ],
					"candicane4" : [ 0.44088, 0.62161, 0.07375, 1.0 ],
					"thickness" : 2,
					"orientation" : 1,
					"spacing" : 0,
					"background" : 0,
					"peakcolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"candicane3" : [ 0.29392, 0.41441, 0.71583, 1.0 ],
					"size" : 3,
					"drawpeaks" : 0,
					"ghostbar" : 0,
					"candicane8" : [ 0.02872, 0.45042, 0.50541, 1.0 ],
					"id" : "obj-19",
					"candicane2" : [ 0.14696, 0.2072, 0.35792, 1.0 ],
					"ignoreclick" : 1,
					"setminmax" : [ 50.0, 80.0 ],
					"border_bottom" : 0,
					"slidercolor" : [ 0.360784, 0.360784, 0.360784, 1.0 ],
					"candycane" : 1,
					"border_top" : 0,
					"candicane7" : [ 0.88176, 0.24321, 0.14749, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"contdata" : 0,
					"border_left" : 0,
					"setstyle" : 0,
					"signed" : 0,
					"candicane6" : [ 0.7348, 0.03601, 0.78958, 1.0 ],
					"compatibility" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"patching_rect" : [ 18.0, 101.0, 20.0, 20.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "int" ],
					"background" : 0,
					"id" : "obj-43",
					"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p ask",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 18.0, 139.0, 47.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 652.0, 100.0, 536.0, 633.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 652.0, 100.0, 536.0, 633.0 ],
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
									"text" : "mousestate",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 5,
									"patching_rect" : [ 180.0, 140.0, 83.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int", "int", "int", "int", "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 2",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 280.0, 220.0, 70.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-33",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 2",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 380.0, 220.0, 70.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-32",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t 1",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 180.0, 260.0, 26.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-31",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "delay 1200",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 180.0, 220.0, 79.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 1",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 40.0, 260.0, 82.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-29",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "gate",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 40.0, 360.0, 59.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b b 0",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 4,
									"patching_rect" : [ 180.0, 180.0, 319.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang", "bang", "bang", "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-27",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
									"patching_rect" : [ 140.0, 260.0, 20.0, 20.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"id" : "obj-23",
									"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl reg",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 2,
									"patching_rect" : [ 20.0, 540.0, 79.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "", "" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 2,
									"patching_rect" : [ 20.0, 180.0, 79.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang", "bang" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "* 127",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 220.0, 460.0, 43.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
									"patching_rect" : [ 220.0, 420.0, 20.0, 20.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"id" : "obj-12",
									"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "* 127",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 100.0, 460.0, 43.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
									"patching_rect" : [ 100.0, 420.0, 20.0, 20.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"id" : "obj-9",
									"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pak",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 100.0, 500.0, 139.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 600",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 60.0, 74.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"id" : "obj-1",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 0,
									"hidden" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 580.0, 25.0, 25.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-38",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-7", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-8", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-32", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 1 ],
									"destination" : [ "obj-33", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 2 ],
									"destination" : [ "obj-32", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 3 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 489.5, 326.0, 49.5, 326.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 189.5, 326.0, 49.5, 326.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 49.5, 401.0, 229.5, 401.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 49.5, 401.0, 109.5, 401.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-28", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-38", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [ 29.5, 115.0, 189.5, 115.0 ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p learn",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 78.0, 139.0, 53.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 0,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 844.0, 44.0, 419.0, 450.0 ],
						"bglocked" : 0,
						"defrect" : [ 844.0, 44.0, 419.0, 450.0 ],
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
									"maxclass" : "button",
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 20.0, 20.0, 20.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-3",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "% 2",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 100.0, 63.0, 23.0 ],
									"outlettype" : [ "int" ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "1. -0.75",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 80.0, 340.0, 58.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "target 0. 1. -1., learn 1. -0.75",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 60.0, 300.0, 188.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 0 1",
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"patching_rect" : [ 20.0, 140.0, 147.0, 23.0 ],
									"outlettype" : [ "bang", "bang", "" ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 1000",
									"fontsize" : 14.0,
									"numoutlets" : 3,
									"patching_rect" : [ 20.0, 60.0, 63.0, 23.0 ],
									"outlettype" : [ "bang", "bang", "int" ],
									"id" : "obj-29",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "target 71 72 62, learn 127 127",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 200.0, 220.0, 198.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "target 60 67 69, learn 0 0",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 220.0, 167.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-17",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 408.0, 25.0, 25.0 ],
									"id" : "obj-36",
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [ 209.5, 282.0, 29.5, 282.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 2 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 1 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"patching_rect" : [ 78.0, 207.0, 20.0, 20.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "int" ],
					"background" : 0,
					"id" : "obj-27",
					"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "error $1",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 78.0, 237.0, 60.0, 21.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-28",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print ERROR",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 98.0, 372.0, 92.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route list",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 2,
					"patching_rect" : [ 18.0, 317.0, 63.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "", "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"hbgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"triscale" : 1.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 2,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"patching_rect" : [ 306.0, 101.0, 50.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"cantchange" : 0,
					"htextcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"triangle" : 1,
					"outlettype" : [ "float", "bang" ],
					"maximum" : 5.0,
					"outputonclick" : 0,
					"background" : 0,
					"mouseup" : 0,
					"minimum" : 0.0,
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"numdecimalplaces" : 0,
					"id" : "obj-15",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "threshold $1",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 306.0, 140.0, 88.0, 21.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-11",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p idiot",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 18.0, 277.0, 48.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 120.0, 77.0, 815.0, 617.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 120.0, 77.0, 815.0, 617.0 ],
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
									"text" : "% 4",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 415.0, 378.0, 35.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "All others are \"tanh\" node.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 447.0, 413.0, 221.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-3",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "-",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 178.0, 276.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.92, 0.94, 0.67, 1.0 ],
									"id" : "obj-25",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "[pack] is \"sigmoid\" node.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 33.0, 393.0, 216.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-24",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "[+] is \"linear\" loop node.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 512.0, 330.0, 200.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-10",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "\"feedback\" synapse delay the signal.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 476.0, 156.0, 304.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-7",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "[ - ] is \"linear\" node.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 73.0, 320.0, 178.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-6",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "dbl click [papa] to rebuild the NN.",
									"fontface" : 0,
									"frgb" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"fontsize" : 18.0,
									"underline" : 0,
									"textcolor" : [ 0.392157, 0.278431, 0.376471, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 296.0, 491.0, 280.0, 27.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-183",
									"fontname" : "Arial",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "+",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 443.0, 321.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.92, 0.94, 0.67, 1.0 ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 360.0, 361.0, 37.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 303.0, 361.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 300.0, 280.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 223.0, 361.0, 40.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.68, 0.82, 0.82, 1.0 ],
									"id" : "obj-17",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "-",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 242.0, 283.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.92, 0.94, 0.67, 1.0 ],
									"id" : "obj-16",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 363.0, 221.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 411.0, 281.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 303.0, 221.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "int",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 223.0, 221.0, 32.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 229.0, 528.0, 25.0, 25.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-5",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"patching_rect" : [ 244.0, 48.0, 25.0, 25.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"id" : "obj-4",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 0,
									"hidden" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "papa 3",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 4,
									"patching_rect" : [ 263.0, 421.0, 74.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "", "", "", "" ],
									"background" : 0,
									"color" : [ 0.83, 0.74, 0.84, 1.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 3,
									"hidden" : 0,
									"head_papa_data" : [ 29, 5, 3, 3, 4, 5, 3 ],
									"graf_papa_data" : [ 0, 1, 0, 1, 1, 0, 2, 1, 0, 11, 1, 2, 12, 1, 2, 14, 1, 2, 13, 1, 2, 15, 1, 0, 17, 1, 2, 26, 1, 0, 16, 1, 1, 18, 1, 2, 19, 1, 2, 20, 0, 0, 28, 1, 2, 3, 1, 2, 4, 1, 2, 5, 1, 2 ],
									"synapses_papa_data" : [ 0, 11, 0, 0.663116, 0, 12, 0, -0.848461, 0, 14, 0, 0.909155, 1, 11, 0, 0.145367, 1, 12, 0, -0.300081, 1, 14, 0, -0.392827, 2, 14, 0, 0.642964, 2, 12, 0, 1.04865, 2, 11, 0, -0.569452, 11, 26, 1, 1.0, 11, 13, 0, 0.079967, 11, 15, 0, -0.759046, 11, 17, 0, 0.353534, 12, 13, 0, -0.056297, 12, 15, 0, 0.455949, 12, 17, 0, -0.716668, 14, 13, 0, 0.951401, 14, 15, 0, -0.278098, 14, 17, 0, 0.841714, 13, 16, 0, 0.017716, 13, 19, 0, 0.08875, 13, 18, 0, 0.517077, 13, 20, 1, 1.0, 13, 28, 0, 0.092147, 15, 16, 0, -0.172301, 15, 18, 0, -0.207307, 15, 19, 0, -0.089882, 17, 16, 0, -0.809168, 17, 19, 0, 0.029861, 17, 18, 0, -0.176486, 26, 11, 1, 1.0, 16, 3, 0, -0.00957, 16, 4, 0, 0.203702, 16, 5, 0, 0.095338, 18, 5, 0, -0.189569, 18, 4, 0, -0.247163, 18, 3, 0, 0.65677, 19, 5, 0, 0.082736, 19, 4, 0, 0.77854, 19, 3, 0, -0.462765, 20, 13, 1, 1.0, 28, 5, 0, -0.408697, 5, 2, 1, 1.0 ],
									"saved_object_attributes" : 									{
										"sigmoid" : [ 0.68, 0.82, 0.82, 1.0 ],
										"tanh" : [ 0.83, 0.74, 0.84, 1.0 ],
										"bias" : 0.0,
										"error" : 0,
										"papafunction" : 2,
										"learning" : 0.5,
										"wrong" : [ 0.85, 0.85, 0.85, 1.0 ],
										"oscarfunction" : 0,
										"linear" : [ 0.92, 0.94, 0.67, 1.0 ],
										"threshold" : 0.0,
										"feedback" : [ 1.0, 0.6, 0.6, 1.0 ],
										"synapse" : [ 0.62, 0.55, 0.83, 1.0 ],
										"embed" : 1
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "oscar 3",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 3,
									"patching_rect" : [ 263.0, 161.0, 77.5, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "", "", "" ],
									"background" : 0,
									"color" : [ 0.92, 0.94, 0.67, 1.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 4,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "T - 2",
									"fontface" : 0,
									"frgb" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"fontsize" : 16.0,
									"underline" : 0,
									"textcolor" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 179.0, 201.0, 46.0, 29.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-27",
									"fontname" : "Comic Sans MS",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "T - 1",
									"fontface" : 0,
									"frgb" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"fontsize" : 16.0,
									"underline" : 0,
									"textcolor" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 300.0, 131.0, 49.0, 29.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-20",
									"fontname" : "Comic Sans MS",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "T - 1",
									"fontface" : 0,
									"frgb" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"fontsize" : 16.0,
									"underline" : 0,
									"textcolor" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 401.0, 246.0, 43.0, 29.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"presentation" : 0,
									"background" : 0,
									"id" : "obj-12",
									"fontname" : "Comic Sans MS",
									"ignoreclick" : 1,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-2", 2 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"color" : [ 0.85, 0.85, 0.85, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-2", 2 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-2", 2 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"color" : [ 1.0, 0.6, 0.6, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 1.0, 0.6, 0.6, 1.0 ],
									"midpoints" : [ 471.0, 369.0, 531.0, 317.0, 465.0, 249.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-2", 2 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"color" : [ 1.0, 0.6, 0.6, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 1.0, 0.6, 0.6, 1.0 ],
									"midpoints" : [ 163.0, 312.0, 96.0, 210.0, 194.0, 176.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"color" : [ 0.85, 0.85, 0.85, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"color" : [ 0.62, 0.55, 0.83, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 3 ],
									"destination" : [ "obj-1", 3 ],
									"hidden" : 0,
									"color" : [ 1.0, 0.6, 0.6, 1.0 ],
									"midpoints" : [ 367.0, 472.0, 471.0, 393.0, 506.0, 214.0, 382.0, 127.0 ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"hbgcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontface" : 0,
					"fontsize" : 14.0,
					"triscale" : 1.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 2,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"patching_rect" : [ 226.0, 101.0, 50.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"cantchange" : 0,
					"htextcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"triangle" : 1,
					"outlettype" : [ "float", "bang" ],
					"maximum" : 5.0,
					"outputonclick" : 0,
					"background" : 0,
					"mouseup" : 0,
					"minimum" : 0.0,
					"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
					"numdecimalplaces" : 0,
					"id" : "obj-7",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "learning $1",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 226.0, 140.0, 80.0, 21.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-5",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 179.0, 140.0, 46.0, 21.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-48",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MIDI",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 18.0, 372.0, 52.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"background" : 0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 727.0, 123.0, 340.0, 201.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 727.0, 123.0, 340.0, 201.0 ],
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
									"text" : "p",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 80.0, 42.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"id" : "obj-14",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0,
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 5,
											"minor" : 1,
											"revision" : 9
										}
,
										"rect" : [ 179.0, 50.0, 340.0, 625.0 ],
										"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
										"bglocked" : 0,
										"defrect" : [ 179.0, 50.0, 340.0, 625.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 80.0, 220.0, 26.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "bang" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-19",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int 60",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 260.0, 45.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "int" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-16",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "split 55 79",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 2,
													"patching_rect" : [ 20.0, 180.0, 79.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "int", "int" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 3,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pipe 500",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 260.0, 340.0, 64.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-5",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "print FLOAT",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 0,
													"patching_rect" : [ 80.0, 140.0, 86.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-17",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numoutlets" : 0,
													"patching_rect" : [ 20.0, 580.0, 25.0, 25.0 ],
													"presentation" : 0,
													"background" : 0,
													"id" : "obj-14",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0,
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 540.0, 40.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "+ 40",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 60.0, 500.0, 38.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "int" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-10",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "drunk 40 20",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 60.0, 460.0, 84.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "int" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 3,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t i b",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 2,
													"patching_rect" : [ 20.0, 420.0, 59.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "int", "bang" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pipe 200",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 100.0, 340.0, 64.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-4",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pipe 400",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 180.0, 340.0, 64.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 2,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "cycle 4",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 4,
													"patching_rect" : [ 20.0, 300.0, 258.5, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "", "", "", "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "route int",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 2,
													"patching_rect" : [ 20.0, 100.0, 61.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "", "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-3",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "iter",
													"fontface" : 0,
													"fontsize" : 14.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 60.0, 30.0, 23.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"color" : [ 0.8, 0.84, 0.71, 1.0 ],
													"id" : "obj-2",
													"fontname" : "Arial",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 1,
													"hidden" : 0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"presentation" : 0,
													"outlettype" : [ "" ],
													"background" : 0,
													"id" : "obj-13",
													"ignoreclick" : 0,
													"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
													"numinlets" : 0,
													"hidden" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [ 189.5, 395.0, 29.5, 395.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 2 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 3 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [ 269.5, 395.0, 29.5, 395.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 1 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 1 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 1 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-11", 1 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 1 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontface" : 0,
										"fontsize" : 14.0,
										"default_fontsize" : 14.0,
										"fontname" : "Arial",
										"default_fontface" : 0,
										"globalpatchername" : ""
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "freebang",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 340.0, 40.0, 66.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "i 0",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 340.0, 120.0, 36.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 16 1",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 3,
									"patching_rect" : [ 340.0, 80.0, 72.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "bang", "bang", "int" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-17",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ctlout 123",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 340.0, 160.0, 72.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 3,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 11",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 220.0, 120.0, 89.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-16",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 180.0, 120.0, 31.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 1,
									"hidden" : 0,
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
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 60.0, 32.5, 23.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"numinlets" : 2
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numoutlets" : 0,
													"patching_rect" : [ 20.0, 360.0, 25.0, 25.0 ],
													"id" : "obj-7",
													"numinlets" : 1,
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack",
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 320.0, 40.0, 23.0 ],
													"outlettype" : [ "" ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"numinlets" : 2
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"fontsize" : 14.0,
													"numoutlets" : 2,
													"patching_rect" : [ 20.0, 220.0, 40.0, 23.0 ],
													"outlettype" : [ "bang", "int" ],
													"id" : "obj-5",
													"fontname" : "Arial",
													"numinlets" : 1
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "uzi 16",
													"fontsize" : 14.0,
													"numoutlets" : 3,
													"patching_rect" : [ 20.0, 140.0, 48.0, 23.0 ],
													"outlettype" : [ "bang", "bang", "int" ],
													"id" : "obj-4",
													"fontname" : "Arial",
													"numinlets" : 2
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "int",
													"fontsize" : 14.0,
													"numoutlets" : 1,
													"patching_rect" : [ 60.0, 280.0, 32.5, 23.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-3",
													"fontname" : "Arial",
													"numinlets" : 2
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b i",
													"fontsize" : 14.0,
													"numoutlets" : 2,
													"patching_rect" : [ 20.0, 100.0, 73.0, 23.0 ],
													"outlettype" : [ "bang", "int" ],
													"id" : "obj-2",
													"fontname" : "Arial",
													"numinlets" : 1
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numoutlets" : 1,
													"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
													"outlettype" : [ "int" ],
													"id" : "obj-1",
													"numinlets" : 0,
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
										"default_fontname" : "Arial",
										"fontface" : 0,
										"fontsize" : 14.0,
										"default_fontsize" : 14.0,
										"fontname" : "Arial",
										"default_fontface" : 0,
										"globalpatchername" : ""
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"align" : 1,
									"fontface" : 0,
									"menumode" : 0,
									"fontsize" : 16.0,
									"underline" : 0,
									"arrowcolor" : [ 0.22, 0.22, 0.22, 1.0 ],
									"textcolor" : [ 0.15, 0.15, 0.15, 1.0 ],
									"numoutlets" : 3,
									"togcolor" : [ 0.55, 0.55, 0.55, 1.0 ],
									"patching_rect" : [ 180.0, 60.0, 152.0, 25.0 ],
									"bgcolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
									"prefix_mode" : 0,
									"arrowlink" : 1,
									"prefix" : "",
									"presentation" : 0,
									"items" : [ "Grand", "Piano", ",", "Bright", "Piano", ",", "Electric", "Grand", ",", "Honkytonk", "Piano", ",", "Electric", "Piano", 1, ",", "Electric", "Piano", 2, ",", "Harpsichord", ",", "Clavinet", ",", "Celesta", ",", "Glockenspiel", ",", "Music", "Box", ",", "Vibraphone", ",", "Marimba", ",", "Xylophone", ",", "Tubular", "Bell", ",", "Dulcimer", ",", "Drawbar", "Organ", ",", "Percussive", "Organ", ",", "Rock", "Organ", ",", "Church", "Organ", ",", "Reed", "Organ", ",", "Accordion", ",", "Harmonica", ",", "Tango", "Accordion", ",", "Nylon", "String", "Guitar", ",", "Steel", "String", "Guitar", ",", "Jazz", "Guitar", ",", "Clean", "Guitar", ",", "Muted", "Guitar", ",", "Overdrive", "Guitar", ",", "Distortion", "Guitar", ",", "Guitar", "Harmonics", ",", "Acoustic", "Bass", ",", "Fingered", "Bass", ",", "Picked", "Bass", ",", "Fretless", "Bass", ",", "Slap", "Bass", 1, ",", "Slap", "Bass", 2, ",", "Synth", "Bass", 1, ",", "Synth", "Bass", 2, ",", "Violin", ",", "Viola", ",", "Cello", ",", "Contrabass", ",", "Tremolo", "Strings", ",", "Pizzicato", "Strings", ",", "Harp", ",", "Timpani", ",", "String", "Ensemble", ",", "Slow", "Strings", ",", "Synth", "Strings", 1, ",", "Synth", "Strings", 2, ",", "Choir", "Aahs", ",", "Voice", "Oohs", ",", "Synth", "Vox", ",", "Orchestra", "Hit", ",", "Trumpet", ",", "Trombone", ",", "Tuba", ",", "Muted", "Trumpet", ",", "French", "Horn", ",", "Brass", 1, ",", "Synth", "Brass", 1, ",", "Synth", "Brass", 2, ",", "Soprano", "Sax", ",", "Alto", "Sax", ",", "Tenor", "Sax", ",", "Baritone", "Sax", ",", "Oboe", ",", "English", "Horn", ",", "Bassoon", ",", "Clarinet", ",", "Piccolo", ",", "Flute", ",", "Recorder", ",", "Pan", "Flute", ",", "Blown", "Bottle", ",", "Shakuhachi", ",", "Whistle", ",", "Ocarina", ",", "Square", "Wave", ",", "Sawtooth", "Wave", ",", "Synth", "Calliope", ",", "Chiffer", "Lead", ",", "Charang", ",", "Solo", "Vox", ",", "5th", "Sawtooth", "Wave", ",", "Bass", "and", "Lead", ",", "Fantasia", ",", "Warm", "Pad", ",", "Polysynth", ",", "Space", "Voice", ",", "Bowed", "Glass", ",", "Metal", "Pad", ",", "Halo", "Pad", ",", "Sweep", "Pad", ",", "Ice", "Rain", ",", "Soundtrack", ",", "Crystal", ",", "Atmosphere", ",", "Brightness", ",", "Goblin", ",", "Echo", "Drops", ",", "Star", "Theme", ",", "Sitar", ",", "Banjo", ",", "Shamisen", ",", "Koto", ",", "Kalimba", ",", "Bag", "Pipe", ",", "Fiddle", ",", "Shanai", ",", "Tinkle", "Bell", ",", "Agogo", ",", "Steel", "Drums", ",", "Woodblock", ",", "Taiko", ",", "Melo", "Tom", ",", "Synth", "Drum", ",", "Reverse", "Cymbal", ",", "Guitar", "Fret", "Noise", ",", "Breath", "Noise", ",", "Seashore", ",", "Bird", ",", "Telephone", ",", "Helicopter", ",", "Applause", ",", "Gun", "Shot" ],
									"pattrmode" : 0,
									"showdotfiles" : 0,
									"outlettype" : [ "int", "", "" ],
									"arrowframe" : 0,
									"rounded" : 8,
									"bgcolor2" : [ 0.917647, 0.937255, 0.670588, 1.0 ],
									"textcolor2" : [ 0.0, 0.0, 0.0, 1.0 ],
									"background" : 0,
									"id" : "obj-7",
									"fontname" : "Arial",
									"framecolor" : [ 0.827451, 0.827451, 0.827451, 0.0 ],
									"truncate" : 1,
									"ignoreclick" : 0,
									"arrowbgcolor" : [ 0.86, 0.86, 0.86, 1.0 ],
									"autopopulate" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"labelclick" : 0,
									"types" : [  ],
									"depth" : 0,
									"numinlets" : 1,
									"discolor" : [ 0.43, 0.43, 0.43, 1.0 ],
									"hltcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"hidden" : 0,
									"arrow" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pgmout",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 180.0, 160.0, 57.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-368",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 2,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 90 250 1",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 3,
									"patching_rect" : [ 20.0, 120.0, 138.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"outlettype" : [ "float", "float", "float" ],
									"background" : 0,
									"color" : [ 0.8, 0.84, 0.71, 1.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 4,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontface" : 0,
									"fontsize" : 14.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 160.0, 138.0, 23.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"presentation" : 0,
									"background" : 0,
									"color" : [ 0.501961, 0.717647, 0.764706, 1.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 3,
									"hidden" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"presentation" : 0,
									"outlettype" : [ "" ],
									"background" : 0,
									"id" : "obj-1",
									"ignoreclick" : 0,
									"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
									"numinlets" : 0,
									"hidden" : 0,
									"comment" : "(list)"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 1,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 1,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 2 ],
									"destination" : [ "obj-19", 2 ],
									"hidden" : 1,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-368", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 1 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 2 ],
									"destination" : [ "obj-4", 2 ],
									"hidden" : 0,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 1,
									"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 139.0, 140.0, 41.0, 21.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ],
					"presentation" : 0,
					"outlettype" : [ "" ],
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"background" : 0,
					"id" : "obj-9",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"gradient" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Papa / Oscar",
					"fontface" : 3,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 20.871338,
					"underline" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 13.0, 13.0, 368.0, 30.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"presentation" : 0,
					"background" : 0,
					"id" : "obj-14",
					"fontname" : "Arial",
					"ignoreclick" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"fontface" : 0,
					"fontsize" : 14.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 2,
					"patching_rect" : [ 222.0, 184.0, 51.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "", "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"ignoreclick" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 0,
					"hidden" : 1,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 845.0, 132.0, 190.0, 198.0 ],
						"bglocked" : 0,
						"defrect" : [ 845.0, 132.0, 190.0, 198.0 ],
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
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 20.0, 68.0, 23.0 ],
									"outlettype" : [ "bang" ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "71 72 62",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 100.0, 100.0, 65.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "60 67 69",
									"fontsize" : 14.0,
									"numoutlets" : 1,
									"patching_rect" : [ 20.0, 100.0, 65.0, 21.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 140.0, 25.0, 25.0 ],
									"id" : "obj-8",
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 100.0, 140.0, 25.0, 25.0 ],
									"id" : "obj-9",
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [ 29.5, 70.0, 109.5, 70.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontface" : 0,
						"fontsize" : 14.0,
						"default_fontsize" : 14.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"settype" : 1,
					"numoutlets" : 2,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 220.0, 227.0, 166.0, 171.0 ],
					"border_right" : 0,
					"bgcolor" : [ 0.858824, 0.858824, 0.858824, 0.0 ],
					"candicane5" : [ 0.58784, 0.82881, 0.43166, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "", "" ],
					"candicane4" : [ 0.44088, 0.62161, 0.07375, 1.0 ],
					"thickness" : 2,
					"orientation" : 1,
					"spacing" : 0,
					"background" : 0,
					"peakcolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"candicane3" : [ 0.29392, 0.41441, 0.71583, 1.0 ],
					"size" : 3,
					"drawpeaks" : 0,
					"ghostbar" : 0,
					"candicane8" : [ 0.02872, 0.45042, 0.50541, 1.0 ],
					"id" : "obj-13",
					"candicane2" : [ 0.14696, 0.2072, 0.35792, 1.0 ],
					"ignoreclick" : 1,
					"setminmax" : [ 50.0, 80.0 ],
					"border_bottom" : 0,
					"slidercolor" : [ 0.615686, 0.552941, 0.827451, 1.0 ],
					"candycane" : 1,
					"border_top" : 0,
					"candicane7" : [ 0.88176, 0.24321, 0.14749, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"contdata" : 0,
					"border_left" : 0,
					"setstyle" : 0,
					"signed" : 0,
					"candicane6" : [ 0.7348, 0.03601, 0.78958, 1.0 ],
					"compatibility" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"settype" : 1,
					"numoutlets" : 2,
					"bordercolor" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"patching_rect" : [ 220.0, 227.0, 166.0, 171.0 ],
					"border_right" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"candicane5" : [ 0.58784, 0.82881, 0.43166, 1.0 ],
					"presentation" : 0,
					"outlettype" : [ "", "" ],
					"candicane4" : [ 0.44088, 0.62161, 0.07375, 1.0 ],
					"thickness" : 2,
					"orientation" : 1,
					"spacing" : 0,
					"background" : 0,
					"peakcolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"candicane3" : [ 0.29392, 0.41441, 0.71583, 1.0 ],
					"size" : 3,
					"drawpeaks" : 0,
					"ghostbar" : 0,
					"candicane8" : [ 0.02872, 0.45042, 0.50541, 1.0 ],
					"id" : "obj-10",
					"candicane2" : [ 0.14696, 0.2072, 0.35792, 1.0 ],
					"ignoreclick" : 1,
					"setminmax" : [ 50.0, 80.0 ],
					"border_bottom" : 1,
					"slidercolor" : [ 0.929412, 0.584314, 1.0, 1.0 ],
					"candycane" : 1,
					"border_top" : 1,
					"candicane7" : [ 0.88176, 0.24321, 0.14749, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0,
					"contdata" : 0,
					"border_left" : 1,
					"setstyle" : 0,
					"signed" : 0,
					"candicane6" : [ 0.7348, 0.03601, 0.78958, 1.0 ],
					"compatibility" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Neural Network (MLP Feedforward / Jordan)",
					"fontface" : 0,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 14.0,
					"underline" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 13.0, 43.0, 332.0, 23.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
					"presentation" : 0,
					"background" : 0,
					"id" : "obj-4",
					"fontname" : "Arial",
					"ignoreclick" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"mode" : 1,
					"numoutlets" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"border" : 0,
					"patching_rect" : [ 10.0, 9.0, 388.0, 65.0 ],
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"angle" : 0.0,
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"presentation" : 0,
					"rounded" : 8,
					"shadow" : 0,
					"background" : 1,
					"id" : "obj-23",
					"ignoreclick" : 1,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"hidden" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 188.5, 192.5, 27.5, 192.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 148.5, 192.5, 27.5, 192.5 ]
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
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 1 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 235.5, 192.0, 27.5, 192.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 315.5, 192.0, 27.5, 192.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-24", 1 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 1,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
