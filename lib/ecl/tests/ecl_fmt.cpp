/*
   Copyright (C) 2012  Equinor ASA, Norway.

   The file 'ecl_fmt.c' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <ert/util/util.h>
#include <ert/util/test_work_area.hpp>
#include <ert/util/test_util.hpp>

#include <ert/ecl/ecl_util.hpp>

void test_content( const ecl::util::TestArea& ta , const char * src_file , bool fmt_file ) {
  ta.copy_file(src_file);
  {
    char * base_name;
    char * extension;
    bool fmt;

    util_alloc_file_components( src_file , NULL , &base_name , &extension);
    char * file_name = util_alloc_filename( NULL , base_name , extension );

    util_copy_file(file_name, base_name);

    test_assert_true( ecl_util_fmt_file( base_name , &fmt ));
    test_assert_bool_equal( fmt , fmt_file );

    free(file_name);
    free(base_name);
    free(extension);
  }
}





void test_small( ) {
  bool fmt;

  FILE * stream = util_fopen("small.txt" , "w");
  fprintf(stream , "Some bytes\n");
  fclose( stream );

  test_assert_false( ecl_util_fmt_file( "small.txt" , &fmt ));
}





int main(int argc , char ** argv) {
  ecl::util::TestArea ta("ecl_fmt");
  {
    const char * binary_file = argv[1];
    const char * text_file = argv[2];


    bool fmt_file;

    test_assert_true( ecl_util_fmt_file( binary_file , &fmt_file ));
    test_assert_false( fmt_file );

    test_assert_true( ecl_util_fmt_file( text_file , &fmt_file ));
    test_assert_true( fmt_file );

    test_assert_true( ecl_util_fmt_file( "TEST.EGRID" , &fmt_file ));
    test_assert_false( fmt_file );

    test_assert_true( ecl_util_fmt_file( "TEST.FEGRID" , &fmt_file ));
    test_assert_true( fmt_file );

    test_assert_false(ecl_util_fmt_file( "TEST_DOES_NOT_EXIST" , &fmt_file ));

    test_content( ta , binary_file , false );
    test_content( ta , text_file , true );
    test_small( );
  }
  exit(0);
}
