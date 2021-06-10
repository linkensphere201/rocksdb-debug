//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
#ifndef ROCKSDB_LITE

#include <sstream>
#include "rocksdb/ldb_tool.h"

class MySliceFormater final : public rocksdb::SliceFormatter {
    public:
        MySliceFormater() = default;
        virtual std::string Format(const rocksdb::Slice& s) const override;
};

std::string
MySliceFormater::Format(const rocksdb::Slice &s) const {
    std::stringstream ss;
    for (decltype(s.size_) i = 0; i < s.size_; i++) {
        if (::isprint(s.data_[i]) == 0) {
            ss << "\\x" << int(s.data_[i]);
        } else {
            ss << s.data_[i];
        }
    }
    ss << " | ";
    for (decltype(s.size_) i = 0; i < s.size_; i++) {
        char buf[10];
        sprintf(buf, "%2d ", int(s.data_[i]));
        ss << buf;
    }
    return ss.str();
}

int main(int argc, char** argv) {
  rocksdb::LDBTool tool;
  rocksdb::LDBOptions lopt;

  lopt.key_formatter = std::make_shared<MySliceFormater>();
  lopt.val_formatter = std::make_shared<MySliceFormater>();
  lopt.print_help_header = "ldb ywj modified";

  tool.Run(argc, argv, rocksdb::Options(), lopt);
  return 0;
}
#else
#include <stdio.h>
int main(int /*argc*/, char** /*argv*/) {
  fprintf(stderr, "Not supported in lite mode.\n");
  return 1;
}
#endif  // ROCKSDB_LITE
