{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 167.0, 118.0, 383.0, 287.0 ],
		"bglocked" : 0,
		"defrect" : [ 167.0, 118.0, 383.0, 287.0 ],
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
					"maxclass" : "tralala",
					"patching_rect" : [ 100.0, 180.0, 50.0, 50.0 ],
					"numinlets" : 1,
					"numoutlets" : 4,
					"id" : "obj-1",
					"outlettype" : [ "list", "", "", "bang" ],
					"tralala" : 					{
						"restore" : 						{
							"chance" : [ "chance", 100 ],
							"velocity" : [ "velocity", 0 ],
							"channel" : [ "channel", 1 ],
							"chord" : [ "chord", 0 ],
							"cell" : [ "cell", 12 ],
							"value" : [ "value", 12 ],
							"zone" : [ "zone", 0, 96, 60, 71 ],
							"0" : [ "note", 0, 60, 80, 12, 0 ],
							"bpm" : [ "bpm", 240 ],
							"pattern" : [ "pattern", 1, 0, -2 ],
							"scale" : [ "scale", "B", "dorian" ]
						}
,
						"current" : 						{
							"chance" : [ "chance", 100 ],
							"velocity" : [ "velocity", 0 ],
							"channel" : [ "channel", 1 ],
							"chord" : [ "chord", 0 ],
							"cell" : [ "cell", 12 ],
							"value" : [ "value", 12 ],
							"zone" : [ "zone", 0, 96, 60, 71 ],
							"0" : [ "note", 0, 60, 80, 12, 0 ],
							"bpm" : [ "bpm", 240 ],
							"pattern" : [ "pattern", 1, 0, -2 ],
							"scale" : [ "scale", "B", "dorian" ]
						}

					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "pattern 1 0 -2",
					"patching_rect" : [ 220.0, 120.0, 94.0, 21.0 ],
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
					"text" : "clear",
					"patching_rect" : [ 220.0, 60.0, 41.0, 21.0 ],
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
					"maxclass" : "message",
					"text" : "scale B dorian",
					"patching_rect" : [ 240.0, 200.0, 99.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-4",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "note 0 60",
					"patching_rect" : [ 60.0, 60.0, 69.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-7",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bpm 240",
					"patching_rect" : [ 140.0, 60.0, 65.0, 21.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
