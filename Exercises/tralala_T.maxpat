{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 417.0, 78.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"defrect" : [ 417.0, 78.0, 640.0, 480.0 ],
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
					"text" : "value $1",
					"patching_rect" : [ 100.0, 160.0, 63.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"presentation_rect" : [ 96.0, 155.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setsymbol $1",
					"patching_rect" : [ 420.0, 200.0, 93.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-18",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tosymbol",
					"patching_rect" : [ 420.0, 160.0, 67.0, 23.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-15",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route cell",
					"patching_rect" : [ 420.0, 120.0, 67.0, 23.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"id" : "obj-4",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "cell $1",
					"patching_rect" : [ 20.0, 160.0, 51.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-6",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tralala",
					"patching_rect" : [ 20.0, 200.0, 250.0, 200.0 ],
					"yoffset" : 656,
					"numinlets" : 1,
					"numoutlets" : 5,
					"id" : "obj-2",
					"outlettype" : [ "list", "", "", "bang", "" ],
					"tralala" : 					{

					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "live.tab",
					"varname" : "live.tab",
					"prototypename" : "notevalues",
					"pictures" : [ "1n.svg", "2n.svg", "4n.svg", "8n.svg", "16n.svg" ],
					"patching_rect" : [ 20.0, 100.0, 169.0, 32.0 ],
					"activebgoncolor" : [ 0.917647, 0.94902, 0.054902, 0.0 ],
					"bordercolor" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"focusbordercolor" : [ 0.827451, 0.737255, 0.835294, 0.0 ],
					"parameter_enable" : 1,
					"usepicture" : 1,
					"numinlets" : 1,
					"bgcolor" : [ 0.827451, 0.737255, 0.835294, 1.0 ],
					"numoutlets" : 3,
					"multiline" : 0,
					"activebgcolor" : [ 0.827451, 0.737255, 0.835294, 0.396078 ],
					"id" : "obj-1",
					"outlettype" : [ "", "", "float" ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_order" : 0,
							"parameter_defer" : 0,
							"parameter_speedlim" : 1.0,
							"parameter_steps" : 0,
							"parameter_enum" : [ "96", "48", "24", "12", "6" ],
							"parameter_exponent" : 1.0,
							"parameter_invisible" : 0,
							"parameter_unitstyle" : 0,
							"parameter_annotation_name" : "",
							"parameter_mmax" : 127.0,
							"parameter_mmin" : 0.0,
							"parameter_type" : 2,
							"parameter_initial_enable" : 0,
							"parameter_shortname" : "live.tab",
							"parameter_modmax" : 127.0,
							"parameter_longname" : "live.tab",
							"parameter_modmin" : 0.0,
							"parameter_linknames" : 0,
							"parameter_modmode" : 0,
							"parameter_info" : "",
							"parameter_units" : ""
						}

					}

				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 4 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ],
		"parameters" : 		{
			"obj-1" : [ "live.tab", "live.tab", 0 ]
		}

	}

}
