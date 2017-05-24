/*
    resolver.h -- A simple class for cross-platform path resolution

    Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#ifndef JHTOOLS_RERSOLVER_
#define JHTOOLS_RERSOLVER_




#include "path.h"

namespace jhtools
{
    /**
     * \brief Simple class for resolving paths on Linux/Windows/Mac OS
     *
     * This convenience class looks for a file or directory given its name
     * and a set of search paths. The implementation walks through the
     * search paths in order and stops once the file is found.
     */
    class resolver
    {
        public:
            typedef std::vector<path>::iterator iterator;
            typedef std::vector<path>::const_iterator const_iterator;
            
            resolver()
            {
                paths_.push_back (path::getcwd());
            }
            
            size_t size() const { return paths_.size(); }
            
            iterator begin() { return paths_.begin(); }
            iterator end()   { return paths_.end(); }
            
            const_iterator begin() const { return paths_.begin(); }
            const_iterator end()   const { return paths_.end(); }
            
            void erase (iterator it) { paths_.erase (it); }
            
            void prepend (const path &path) { paths_.insert (paths_.begin(), path); }
            void append (const path &path) { paths_.push_back (path); }
            const path &operator[] (size_t index) const { return paths_[index]; }
            path &operator[] (size_t index) { return paths_[index]; }
            
            path resolve (const path &value) const
            {
                for (const_iterator it = paths_.begin(); it != paths_.end(); ++it)
                {
                    path combined = *it / value;
                    
                    if (combined.exists())
                    { return combined; }
                }
                
                return value;
            }
            
            friend std::ostream &operator<< (std::ostream &os, const resolver &r)
            {
                os << "resolver[" << std::endl;
                
                for (size_t i = 0; i < r.paths_.size(); ++i)
                {
                    os << "  \"" << r.paths_[i] << "\"";
                    
                    if (i + 1 < r.paths_.size())
                    { os << ","; }
                    
                    os << std::endl;
                }
                
                os << "]";
                return os;
            }
            
        private:
            std::vector<path> paths_;
    };
    
    
}//namespace jhtools

#endif // !JHTOOLS_RERSOLVER_