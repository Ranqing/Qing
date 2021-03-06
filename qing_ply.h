#pragma once

#include "qing_common.h"
#include "qing_macros.h"

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
        fprintf( file, "property uchar red\n");
        fprintf( file, "property uchar green\n");
        fprintf( file, "property uchar blue\n");
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

inline void qing_write_xyz(const string &filename, vector<Vec3f> &points)
{
    fstream fout (filename.c_str(), ios::out);
    if(fout.is_open() == false)
    {
        cerr << "failed to open " << filename << endl;
        return ;
    }

    for(int i = 0; i < points.size(); ++i)
    {
        fout << points[i].val[0] << ' ' << points[i].val[1] << ' ' << points[i].val[2] << endl;
    }
    fout.close();
}

inline void qing_write_point_color_ply(const string& plyname, const vector<Vec3f>& points, const vector<Vec3f>& colors)
{
    fstream fout (plyname.c_str(), ios::out);
    if(fout.is_open() == false) {
        cerr << "failed to open " << plyname << endl;
        return ;
    }
    fout << "ply" << endl;
    fout << "format ascii 1.0"     << endl;
    fout << "element vertex "      << points.size() << endl;
    fout << "property float x"     << endl;
    fout << "property float y"     << endl;
    fout << "property float z"     << endl;
    fout << "property uchar red"   << endl;
    fout << "property uchar green" << endl;
    fout << "property uchar blue"  << endl;
    fout << "end_header"           << endl;

    for(int i = 0; i < points.size(); ++i)
    {
        Vec3f p = points[i];
        Vec3f c = colors[i];


        fout << p.val[0] << ' ' << p.val[1] << ' ' << p.val[2] << ' '
             << c.val[0] << ' ' << c.val[1] << ' ' << c.val[2] << endl;

    }
    fout.close();
}
