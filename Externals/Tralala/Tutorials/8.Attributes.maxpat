{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 439.0, 262.0, 375.0, 409.0 ],
		"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
		"bglocked" : 0,
		"defrect" : [ 439.0, 262.0, 375.0, 409.0 ],
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
					"text" : "loadmess set 3",
					"patching_rect" : [ 200.0, 80.0, 105.0, 23.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"hidden" : 1,
					"id" : "obj-8",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "live.tab",
					"varname" : "live.tab",
					"prototypename" : "notevalues",
					"patching_rect" : [ 180.0, 165.0, 169.0, 32.0 ],
					"spacing_x" : 6.0,
					"bgoncolor" : [ 0.815686, 0.847059, 0.886275, 1.0 ],
					"mode" : 0,
					"presentation" : 0,
					"usepicture" : 1,
					"numoutlets" : 3,
					"parameter_enable" : 1,
					"fontface" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"button" : 0,
					"fontsize" : 10.0,
					"spacing_y" : 6.0,
					"textoncolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "", "", "float" ],
					"background" : 0,
					"multiline" : 0,
					"blinktime" : 150,
					"activebgcolor" : [ 0.827451, 0.737255, 0.835294, 0.396078 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"pictures" : [ "1n.svg", "2n.svg", "4n.svg", "8n.svg", "16n.svg" ],
					"fontname" : "Arial",
					"bordercolor" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"activebgoncolor" : [ 0.917647, 0.94902, 0.054902, 0.0 ],
					"hidden" : 0,
					"focusbordercolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
					"id" : "obj-1",
					"numinlets" : 1,
					"bgcolor" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "live.tab",
							"parameter_modmin" : 0.0,
							"parameter_linknames" : 0,
							"parameter_modmode" : 0,
							"parameter_info" : "",
							"parameter_units" : "",
							"parameter_order" : 0,
							"parameter_defer" : 0,
							"parameter_speedlim" : 1.0,
							"parameter_steps" : 0,
							"parameter_invisible" : 0,
							"parameter_enum" : [ "96", "48", "24", "12", "6" ],
							"parameter_exponent" : 1.0,
							"parameter_annotation_name" : "",
							"parameter_unitstyle" : 0,
							"parameter_mmax" : 127.0,
							"parameter_mmin" : 0.0,
							"parameter_type" : 2,
							"parameter_initial_enable" : 0,
							"parameter_shortname" : "live.tab",
							"parameter_modmax" : 127.0
						}

					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "value $1",
					"patching_rect" : [ 255.0, 216.0, 63.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-5",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"patching_rect" : [ 172.0, 352.0, 68.0, 23.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "bang" ],
					"background" : 0,
					"color" : [ 0.8, 0.84, 0.71, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"hidden" : 1,
					"id" : "obj-12",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"patching_rect" : [ 175.0, 260.0, 20.0, 20.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 1.0 ],
					"hidden" : 0,
					"id" : "obj-9",
					"numinlets" : 1,
					"checkedcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "viewtext $1",
					"patching_rect" : [ 175.0, 286.0, 81.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-7",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "scale B natural",
					"patching_rect" : [ 46.0, 286.0, 103.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-18",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "cell quarter",
					"patching_rect" : [ 134.0, 216.0, 80.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-16",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "chord 1",
					"patching_rect" : [ 45.0, 216.0, 57.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-2",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "channel 2",
					"patching_rect" : [ 268.0, 111.0, 71.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-15",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "velocity -10",
					"patching_rect" : [ 172.0, 111.0, 81.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-10",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "chance 50",
					"patching_rect" : [ 87.0, 111.0, 75.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-6",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bpm 240",
					"patching_rect" : [ 18.0, 111.0, 65.0, 21.0 ],
					"presentation" : 0,
					"numoutlets" : 1,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"outlettype" : [ "" ],
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"gradient" : 0,
					"hidden" : 0,
					"id" : "obj-3",
					"numinlets" : 2,
					"bgcolor2" : [ 0.867, 0.867, 0.867, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"patching_rect" : [ 18.0, 360.0, 40.0, 23.0 ],
					"presentation" : 0,
					"numoutlets" : 0,
					"fontface" : 0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 14.0,
					"background" : 0,
					"color" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 0,
					"fontname" : "Arial",
					"hidden" : 0,
					"id" : "obj-11",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 5,
							"minor" : 1,
							"revision" : 9
						}
,
						"rect" : [ 853.0, 125.0, 292.0, 296.0 ],
						"bglocked" : 0,
						"defrect" : [ 853.0, 125.0, 292.0, 296.0 ],
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
									"maxclass" : "tralala",
									"yoffset" : 648,
									"patching_rect" : [ 20.0, 60.0, 250.0, 200.0 ],
									"numoutlets" : 5,
									"outlettype" : [ "list", "", "", "bang", "" ],
									"id" : "obj-1",
									"numinlets" : 1,
									"tralala" : 									{

									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"patching_rect" : [ 20.0, 20.0, 25.0, 25.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-2",
									"numinlets" : 0,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-1", 0 ],
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
					"maxclass" : "comment",
					"text" : "December 28, 2012",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 45.0, 227.0, 23.0 ],
					"presentation" : 0,
					"numoutlets" : 0,
					"underline" : 0,
					"fontface" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 14.0,
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"fontname" : "Arial",
					"hidden" : 0,
					"id" : "obj-4",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Attributes",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 13.0, 13.0, 122.0, 30.0 ],
					"presentation" : 0,
					"numoutlets" : 0,
					"underline" : 0,
					"fontface" : 3,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 20.871338,
					"background" : 0,
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"fontname" : "Arial",
					"hidden" : 0,
					"id" : "obj-14",
					"numinlets" : 1,
					"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"patching_rect" : [ 10.0, 9.0, 355.0, 64.0 ],
					"angle" : 0.0,
					"mode" : 1,
					"presentation" : 0,
					"border" : 0,
					"numoutlets" : 0,
					"rounded" : 8,
					"background" : 1,
					"grad1" : [ 0.188235, 0.105882, 0.266667, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 0.0, 0.0 ],
					"ignoreclick" : 1,
					"shadow" : 0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"grad2" : [ 0.858824, 0.858824, 0.858824, 1.0 ],
					"hidden" : 0,
					"id" : "obj-23",
					"numinlets" : 1,
					"bgcolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 143.5, 256.0, 27.5, 256.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 96.5, 147.5, 27.5, 147.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 277.5, 147.0, 27.5, 147.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 181.5, 147.0, 27.5, 147.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 184.5, 324.0, 27.5, 324.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 264.5, 324.0, 27.5, 324.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"color" : [ 0.694118, 0.694118, 0.694118, 1.0 ],
					"midpoints" : [ 54.5, 256.0, 27.5, 256.0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-1" : [ "live.tab", "live.tab", 0 ]
		}

	}

}
