{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 26.0, 69.0, 335.0, 256.0 ],
		"bglocked" : 0,
		"defrect" : [ 26.0, 69.0, 335.0, 256.0 ],
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
					"text" : "scale B dorian",
					"patching_rect" : [ 180.0, 120.0, 99.0, 21.0 ],
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
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tralala",
					"patching_rect" : [ 80.0, 180.0, 59.5, 23.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 4,
					"id" : "obj-1",
					"outlettype" : [ "list", "", "bang", "bang" ],
					"tralala_data" : 					{
						"current_data" : 						{

						}

					}

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
