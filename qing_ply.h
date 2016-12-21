#pragma once

#include "qing_common.h"

#define PT_UNDEFINED  1.0e38f
#define PT_HAS_NORMAL 1 << 1
#define PT_HAS_COLOR  1 << 2

inline void writePLY(const string fn, const int wid, const int hei, const int validnum, const int flag,
                     const vector<Point3f>& points, const vector<Vec3b>& colors, const vector<Point3f>& normals = vector<Point3f>() )
{
	FILE* file = fopen( fn.c_str(), "w" );
    if ( file == NULL )
	{
		cout << "writePLY() : failed to open " << fn << endl;
		return ;
	}

	fprintf( file, "ply\n" );
	fprintf( file, "format ascii 1.0\n" );
	fprintf( file, "obj_info num_cols %d\n", wid );
	fprintf( file, "obj_info num_rows %d\n", hei );
	fprintf( file, "element vertex %d\n", validnum);
	fprintf( file, "property float x\n");	
	fprintf( file, "property float y\n");	
	fprintf( file, "property float z\n" );

	if ( flag & PT_HAS_COLOR )
	{
		fprintf( file, "property uchar diffuse_red\n");
		fprintf( file, "property uchar diffuse_green\n");
		fprintf( file, "property uchar diffuse_blue\n");
	}
	if ( flag & PT_HAS_NORMAL )
	{
		fprintf( file, "property float nx\n");
		fprintf( file, "property float ny\n");
		fprintf( file, "property float nz\n");
	}
	fprintf( file, "element range_grid %d\n", wid * hei );
	fprintf( file, "property list uchar int vertex_indices\n" );
	fprintf( file, "end_header\n" );

	int index = 0;
	for ( int i = hei - 1; i >= 0; i-- )
	{
		for ( int j = 0; j < wid; j++ )
		{
			index = i * wid + j;
			if ( points[ index ].x != PT_UNDEFINED )
			{
				fprintf( file, "%.4f %.4f %.4f", points[index].x, points[index].y, points[index].z );					
				if ( flag & PT_HAS_COLOR )
					fprintf( file, " %u %u %u",  colors[index].val[0], colors[index].val[1], colors[index].val[2] );	//pcolor:r,g,b
				if ( flag & PT_HAS_NORMAL)
                    fprintf( file, " %.4f %.4f %.4f", normals[index].x, normals[index].y,normals[index].z );

				fprintf (file, "\n");
			}
		}
	}

	index = 0;
	for ( int i = hei - 1; i >= 0; -- i)
	{
		for ( int j = 0; j < wid; ++ j )
		{
			if ( points[ i * wid + j ].x != PT_UNDEFINED )
			{
				fprintf( file, "1 %d\n", index++ );
			}
			else
			{
				fprintf( file, "0\n" );
			}
		}
	}
	
	fclose( file );
    file = NULL;
}

inline void qing_write_xyz(const string& filename, vector<Point3f>& points)
{
    fstream fout (filename.c_str(), ios::out);
    if(fout.is_open() == false)
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    for(int i = 0; i < points.size(); ++i)
    {
        fout << points[i].x << ' ' << points[i].y << ' ' << points[i].z << endl;
    }
    fout.close();
}
